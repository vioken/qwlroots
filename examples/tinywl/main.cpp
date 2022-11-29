// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QProcess>
#include <QSocketNotifier>
#include <QThread>
#include <QAbstractEventDispatcher>
#include <QRect>
#include <QLoggingCategory>

#include <qwbackend.h>
#include <util/qwsignalconnector.h>
#include <render/qwrenderer.h>
#include <render/qwallocator.h>
#include <types/qwcompositor.h>
#include <types/qwsubcompositor.h>
#include <types/qwdatadevice.h>
#include <types/qwoutputlayout.h>
#include <types/qwoutput.h>
#include <types/qwscene.h>
#include <types/qwxdgshell.h>
#include <types/qwcursor.h>
#include <types/qwxcursormanager.h>

#define WLR_USE_UNSTABLE
extern "C" {
#define static
#include <wlr/util/log.h>
#include <wlr/util/edges.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_data_device.h>
#undef static
#include <wayland-server.h>
}

QW_USE_NAMESPACE

class TinywlServer : public QObject
{
    Q_OBJECT
public:
    TinywlServer();
    ~TinywlServer();

    bool start();

private Q_SLOTS:
    void onNewOutput(wlr_output *output);
    void onNewXdgSurface(wlr_xdg_surface *surface);
    void onXdgToplevelMap();
    void onXdgToplevelUnmap();
    void onXdgToplevelRequestMove(wlr_xdg_toplevel_move_event *);
    void onXdgToplevelRequestResize(wlr_xdg_toplevel_resize_event *event);
    void onXdgToplevelRequestMaximize(bool maximize);
    void onXdgToplevelRequestRequestFullscreen(bool fullscreen);

    void onCursorMotion(wlr_pointer_motion_event *event);
    void onCursorMotionAbsolute(wlr_pointer_motion_absolute_event *event);
    void onCursorButton(wlr_pointer_button_event *event);
    void onCursorAxis(wlr_pointer_axis_event *event);
    void onCursorFrame();

    void onNewInput(wlr_input_device *device);
    void onRequestSetCursor(wlr_seat_pointer_request_set_cursor_event *event);
    void onRequestSetSelection(wlr_seat_request_set_selection_event *event);

    void onKeyboardModifiers(void*, wlr_keyboard *keyboard);
    void onKeyboardKey(wlr_keyboard_key_event *event, wlr_keyboard *keyboard);
    void onKeyboardDestroy(void*, wlr_keyboard *keyboard);

    void onOutputFrame();

    void processWaylandEvents();

private:
    struct View
    {
        TinywlServer *server;
        QWXdgToplevel *xdgToplevel;
        QWSceneTree *sceneTree;
        QPointF pos;
    };

    enum class CursorState {
        Normal,
        MovingWindow,
        ResizingWindow,
    };

    static inline View *getView(const QWXdgSurface *surface);
    View *viewAt(const QPointF &pos, wlr_surface **surface, QPointF *spos) const;
    void processCursorMotion(uint32_t time);
    void focusView(View *view, wlr_surface *surface);
    void beginInteractive(View *view, CursorState state, uint32_t edges);
    bool handleKeybinding(xkb_keysym_t sym);

    wl_display *display;
    wl_event_loop *loop;

    QWBackend *backend;
    QWRenderer *renderer;
    QWAllocator *allocator;

    QWCompositor *compositor;
    QWSubcompositor *subcompositor;
    QWDataDeviceManager *dataDeviceManager;

    QWOutputLayout *outputLayout;
    QList<QWOutput*> outputs;

    QWScene *scene;
    QWXdgShell *xdgShell;
    QList<View*> views;
    View *grabbedView = nullptr;
    QPointF grabCursorPos;
    QRectF grabGeoBox;

    QWCursor *cursor;
    QWXCursorManager *cursorManager;
    CursorState cursorState = CursorState::Normal;
    uint32_t resizingEdges = 0;

    QList<wlr_keyboard*> keyboards;
    // TODO: QWSeat
    wlr_seat *seat;
    QWSignalConnector sc;
};

TinywlServer::TinywlServer()
{
    display = wl_display_create();
    backend = QWBackend::autoCreate(display, this);
    if (!backend)
        qFatal("failed to create wlr_backend");

    renderer = QWRenderer::autoCreate(backend);
    if (!renderer)
        qFatal("failed to create wlr_renderer");
    renderer->initWlDisplay(display);

    allocator = QWAllocator::autoCreate(backend, renderer);
    if (!allocator)
        qFatal("failed to create wlr_allocator");

    compositor = QWCompositor::create(display, renderer);
    subcompositor = QWSubcompositor::create(display);
    dataDeviceManager = QWDataDeviceManager::create(display);

    outputLayout = QWOutputLayout::create();
    connect(backend, &QWBackend::newOutput, this, &TinywlServer::onNewOutput);

    scene = QWScene::create();
    scene->attachOutputLayout(outputLayout);
    xdgShell = QWXdgShell::create(display, 3);
    connect(xdgShell, &QWXdgShell::newSurface, this, &TinywlServer::onNewXdgSurface);

    cursor = QWCursor::create();
    cursor->attachOutputLayout(outputLayout);
    cursorManager = QWXCursorManager::create(nullptr, 24);
    cursorManager->load(1);
    connect(cursor, &QWCursor::motion, this, &TinywlServer::onCursorMotion);
    connect(cursor, &QWCursor::motionAbsolute, this, &TinywlServer::onCursorMotionAbsolute);
    connect(cursor, &QWCursor::button, this, &TinywlServer::onCursorButton);
    connect(cursor, &QWCursor::axis, this, &TinywlServer::onCursorAxis);
    connect(cursor, &QWCursor::frame, this, &TinywlServer::onCursorFrame);

    connect(backend, &QWBackend::newInput, this, &TinywlServer::onNewInput);
    seat = wlr_seat_create(display, "seat0");
    sc.connect(&seat->events.request_set_cursor, this, &TinywlServer::onRequestSetCursor);
    sc.connect(&seat->events.request_set_selection, this, &TinywlServer::onRequestSetSelection);
}

TinywlServer::~TinywlServer()
{
    delete backend;
    wl_display_destroy_clients(display);
    wl_display_destroy(display);
}

bool TinywlServer::start()
{
    const char *socket = wl_display_add_socket_auto(display);
    if (!socket) {
        return false;
    }

    if (!backend->start())
        return false;

    qputenv("WAYLAND_DISPLAY", QByteArray(socket));
    qInfo("Running Wayland compositor on WAYLAND_DISPLAY=%s", socket);

    loop = wl_display_get_event_loop(display);
    int fd = wl_event_loop_get_fd(loop);

    auto sockNot = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(sockNot, &QSocketNotifier::activated, this, &TinywlServer::processWaylandEvents);

    QAbstractEventDispatcher *dispatcher = thread()->eventDispatcher();
    connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, this, &TinywlServer::processWaylandEvents);

    return true;
}

void TinywlServer::onNewOutput(wlr_output *output)
{
    Q_ASSERT(output);
    auto o = new QWOutput(output);
    outputs.append(o);

    o->initRender(allocator, renderer);
    if (!wl_list_empty(&output->modes)) {
        auto *mode = o->preferredMode();
        o->setMode(mode);
        o->enable(true);
        if (!o->commit())
            return;
    }

    connect(o, &QWOutput::frame, this, &TinywlServer::onOutputFrame);
    outputLayout->addAuto(o->handle());
}

void TinywlServer::onNewXdgSurface(wlr_xdg_surface *surface)
{
    if (surface->role == WLR_XDG_SURFACE_ROLE_POPUP) {
        auto *s = new QWXdgPopup(surface->popup);
        wlr_xdg_surface *parent = QWXdgSurface::from(surface->popup->parent);
        QWSceneTree *parentTree = reinterpret_cast<QWSceneTree*>(parent->data);
        surface->data = QWScene::xdgSurfaceCreate(parentTree, s);
        return;
    }
    Q_ASSERT(surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);

    auto view = new View();
    view->server = this;
    auto s = new QWXdgToplevel(surface->toplevel);
    view->xdgToplevel = s;
    view->sceneTree = QWScene::xdgSurfaceCreate(scene, s);
    view->sceneTree->handle()->node.data = view;
    surface->data = view->sceneTree;
    connect(s, &QWXdgSurface::map, this, &TinywlServer::onXdgToplevelMap);
    connect(s, &QWXdgSurface::unmap, this, &TinywlServer::onXdgToplevelUnmap);
    connect(s, &QWXdgToplevel::requestMove, this, &TinywlServer::onXdgToplevelRequestMove);
    connect(s, &QWXdgToplevel::requestResize, this, &TinywlServer::onXdgToplevelRequestResize);
    connect(s, &QWXdgToplevel::requestMaximize, this, &TinywlServer::onXdgToplevelRequestMaximize);
    connect(s, &QWXdgToplevel::requestFullscreen, this, &TinywlServer::onXdgToplevelRequestRequestFullscreen);
    connect(s, &QWXdgToplevel::destroyed, this, [this, view] {
        views.removeOne(view);
        if (grabbedView == view)
            grabbedView = nullptr;
        delete view;
    });
}

void TinywlServer::onXdgToplevelMap()
{
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    auto view = getView(surface);
    Q_ASSERT(view);
    views.append(view);
    focusView(view, surface->handle()->surface);
}

void TinywlServer::onXdgToplevelUnmap()
{
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    auto view = getView(surface);
    Q_ASSERT(view);
    views.removeOne(view);
}

void TinywlServer::onXdgToplevelRequestMove(wlr_xdg_toplevel_move_event *)
{
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    auto view = getView(surface);
    Q_ASSERT(view);
    beginInteractive(view, CursorState::MovingWindow, 0);
}

void TinywlServer::onXdgToplevelRequestResize(wlr_xdg_toplevel_resize_event *event)
{
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    auto view = getView(surface);
    Q_ASSERT(view);
    beginInteractive(view, CursorState::ResizingWindow, event->edges);
}

void TinywlServer::onXdgToplevelRequestMaximize(bool maximize)
{
    Q_UNUSED(maximize);
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    surface->scheduleConfigure();
}

void TinywlServer::onXdgToplevelRequestRequestFullscreen(bool fullscreen)
{
    Q_UNUSED(fullscreen);
    auto surface = qobject_cast<QWXdgSurface*>(sender());
    surface->scheduleConfigure();
}

void TinywlServer::onCursorMotion(wlr_pointer_motion_event *event)
{
    cursor->move(&event->pointer->base, QPointF(event->delta_x, event->delta_y));
    processCursorMotion(event->time_msec);
}

void TinywlServer::onCursorMotionAbsolute(wlr_pointer_motion_absolute_event *event)
{
    cursor->warpAbsolute(&event->pointer->base, QPointF(event->x, event->y));
    processCursorMotion(event->time_msec);
}

void TinywlServer::onCursorButton(wlr_pointer_button_event *event)
{
    wlr_seat_pointer_notify_button(seat, event->time_msec, event->button, event->state);
    QPointF spos;
    wlr_surface *surface = nullptr;
    auto view = viewAt(cursor->position(), &surface, &spos);
    if (event->state == WLR_BUTTON_RELEASED) {
        cursorState = CursorState::Normal;
    } else {
        focusView(view, surface);
    }
}

void TinywlServer::onCursorAxis(wlr_pointer_axis_event *event)
{
    wlr_seat_pointer_notify_axis(seat, event->time_msec, event->orientation,
                                 event->delta, event->delta_discrete, event->source);
}

void TinywlServer::onCursorFrame()
{
    wlr_seat_pointer_notify_frame(seat);
}

void TinywlServer::onNewInput(wlr_input_device *device)
{
    if (device->type == WLR_INPUT_DEVICE_KEYBOARD) {
        auto *wlr_keyboard = wlr_keyboard_from_input_device(device);

        xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        xkb_keymap *keymap = xkb_keymap_new_from_names(context, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);

        wlr_keyboard_set_keymap(wlr_keyboard, keymap);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        wlr_keyboard_set_repeat_info(wlr_keyboard, 25, 600);

        sc.connect(&wlr_keyboard->events.modifiers, this, &TinywlServer::onKeyboardModifiers, wlr_keyboard);
        sc.connect(&wlr_keyboard->events.key, this, &TinywlServer::onKeyboardKey, wlr_keyboard);
        sc.connect(&device->events.destroy, this, &TinywlServer::onKeyboardDestroy, wlr_keyboard);

        wlr_seat_set_keyboard(seat, wlr_keyboard);

        Q_ASSERT(!keyboards.contains(wlr_keyboard));
        keyboards.append(wlr_keyboard);
    } else if (device->type == WLR_INPUT_DEVICE_POINTER) {
        cursor->attachInputDevice(device);
    }

    uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
    if (!keyboards.isEmpty()) {
        caps |= WL_SEAT_CAPABILITY_KEYBOARD;
    }
    wlr_seat_set_capabilities(seat, caps);
}

void TinywlServer::onRequestSetCursor(wlr_seat_pointer_request_set_cursor_event *event)
{
    if (seat->pointer_state.focused_client == event->seat_client)
        cursor->setSurface(event->surface, QPoint(event->hotspot_x, event->hotspot_y));
}

void TinywlServer::onRequestSetSelection(wlr_seat_request_set_selection_event *event)
{
    wlr_seat_set_selection(seat, event->source, event->serial);
}

void TinywlServer::onKeyboardModifiers(void *, wlr_keyboard *keyboard)
{
    wlr_seat_set_keyboard(seat, keyboard);
    wlr_seat_keyboard_notify_modifiers(seat, &keyboard->modifiers);
}

void TinywlServer::onKeyboardKey(wlr_keyboard_key_event *event, wlr_keyboard *keyboard)
{
    /* Translate libinput keycode -> xkbcommon */
    uint32_t keycode = event->keycode + 8;
    const xkb_keysym_t *syms;
    int nsyms = xkb_state_key_get_syms(keyboard->xkb_state, keycode, &syms);

    bool handled = false;
    uint32_t modifiers = wlr_keyboard_get_modifiers(keyboard);
    if ((modifiers & (WLR_MODIFIER_ALT | WLR_MODIFIER_CTRL))
            && event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
        for (int i = 0; i < nsyms; i++)
            handled = handleKeybinding(syms[i]);
    }

    if (!handled) {
        wlr_seat_set_keyboard(seat, keyboard);
        wlr_seat_keyboard_notify_key(seat, event->time_msec, event->keycode, event->state);
    }
}

void TinywlServer::onKeyboardDestroy(void *, wlr_keyboard *keyboard)
{
    keyboards.removeOne(keyboard);
    sc.invalidate(&keyboard->events.modifiers);
    sc.invalidate(&keyboard->events.key);
    sc.invalidate(&keyboard->base.events.destroy);
}

void TinywlServer::onOutputFrame()
{
    auto output = qobject_cast<QWOutput*>(sender());
    Q_ASSERT(output);
    auto sceneOutput = QWSceneOutput::get(scene, output);
    wlr_scene_output_commit(sceneOutput);

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    wlr_scene_output_send_frame_done(sceneOutput, &now);
}

void TinywlServer::processWaylandEvents()
{
    int ret = wl_event_loop_dispatch(loop, 0);
    if (ret)
        fprintf(stderr, "wl_event_loop_dispatch error: %d\n", ret);
    wl_display_flush_clients(display);
}

TinywlServer::View *TinywlServer::getView(const QWXdgSurface *surface)
{
    auto sceneTree = reinterpret_cast<QWSceneTree*>(surface->handle()->data);
    return reinterpret_cast<View*>(sceneTree->handle()->node.data);
}

TinywlServer::View *TinywlServer::viewAt(const QPointF &pos, wlr_surface **surface, QPointF *spos) const
{
    auto node = scene->at(pos, spos);
    if (!node || node->type != WLR_SCENE_NODE_BUFFER) {
        return nullptr;
    }
    auto *sceneBuffer = QWSceneBuffer::fromNode(node);
    auto *sceneSurface = wlr_scene_surface_from_buffer(sceneBuffer);
    if (!sceneSurface)
        return nullptr;

    *surface = sceneSurface->surface;
    wlr_scene_tree *tree = node->parent;
    while (tree && !tree->node.data) {
        tree = tree->node.parent;
    }

    return tree ? reinterpret_cast<View*>(tree->node.data) : nullptr;
}

void TinywlServer::processCursorMotion(uint32_t time)
{
    if (cursorState == CursorState::MovingWindow) {
        grabbedView->pos = grabGeoBox.topLeft() + (cursor->position() - grabCursorPos);
        grabbedView->sceneTree->setPosition(grabbedView->pos.toPoint());
        return;
    } else if (cursorState == CursorState::ResizingWindow) {
        const QPointF &cursorPos = cursor->position();
        QRectF newGeoBox = grabGeoBox;
        const int minimumSize = 10;

        if (resizingEdges & WLR_EDGE_TOP) {
            newGeoBox.setTop(cursorPos.y());
        } else if (resizingEdges & WLR_EDGE_BOTTOM) {
            newGeoBox.setBottom(cursorPos.y());
        }
        if (resizingEdges & WLR_EDGE_LEFT) {
            newGeoBox.setLeft(cursorPos.x());
        } else if (resizingEdges & WLR_EDGE_RIGHT) {
            newGeoBox.setRight(cursorPos.x());
        }

        QSize minSize(grabbedView->xdgToplevel->handle()->current.min_width,
                      grabbedView->xdgToplevel->handle()->current.min_height);
        QSize maxSize(grabbedView->xdgToplevel->handle()->current.max_width,
                      grabbedView->xdgToplevel->handle()->current.max_height);

        if (maxSize.width() == 0)
            maxSize.setWidth(99999);
        if (maxSize.height() == 0)
            maxSize.setHeight(99999);

        auto currentGeoBox = grabbedView->xdgToplevel->getGeometry();
        currentGeoBox.moveTopLeft((grabbedView->pos + currentGeoBox.topLeft()).toPoint());
        if (newGeoBox.width() < qMax(minimumSize, minSize.width()) || newGeoBox.width() > maxSize.width()) {
            newGeoBox.setLeft(currentGeoBox.left());
            newGeoBox.setRight(currentGeoBox.right());
        }

        if (newGeoBox.height() < qMax(minimumSize, minSize.height()) || newGeoBox.height() > maxSize.height()) {
            newGeoBox.setTop(currentGeoBox.top());
            newGeoBox.setBottom(currentGeoBox.bottom());
        }

        grabbedView->pos = newGeoBox.topLeft();
        grabbedView->sceneTree->setPosition(grabbedView->pos.toPoint());
        grabbedView->xdgToplevel->setSize(newGeoBox.size().toSize());
        return;
    }

    wlr_surface *surface = nullptr;
    QPointF spos;
    auto view = viewAt(cursor->position(), &surface, &spos);
    if (!view)
        cursorManager->setCursor("left_ptr", cursor);

    if (surface) {
        wlr_seat_pointer_notify_enter(seat, surface, spos.x(), spos.y());
        wlr_seat_pointer_notify_motion(seat, time, spos.x(), spos.y());
    } else {
        wlr_seat_pointer_clear_focus(seat);
    }
}

void TinywlServer::focusView(View *view, wlr_surface *surface)
{
    if (!view)
        return;

    wlr_surface *prevSurface = seat->keyboard_state.focused_surface;
    if (prevSurface == surface) {
        return;
    }
    if (prevSurface) {
        wlr_xdg_surface *previous = QWXdgSurface::from(prevSurface);
        Q_ASSERT(previous->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);
        wlr_xdg_toplevel_set_activated(previous->toplevel, false);
    }

    view->sceneTree->raiseToTop();
    views.move(views.indexOf(view), 0);
    view->xdgToplevel->setActivated(true);

    if (wlr_keyboard *keyboard = wlr_seat_get_keyboard(seat)) {
        wlr_seat_keyboard_notify_enter(seat, view->xdgToplevel->handle()->base->surface,
                                       keyboard->keycodes, keyboard->num_keycodes, &keyboard->modifiers);
    }
}

void TinywlServer::beginInteractive(View *view, CursorState state, uint32_t edges)
{
    wlr_surface *focusedSurface = seat->pointer_state.focused_surface;
    if (view->xdgToplevel->handle()->base->surface !=
            wlr_surface_get_root_surface(focusedSurface)) {
        return;
    }
    grabbedView = view;
    cursorState = state;
    grabCursorPos = cursor->position();
    grabGeoBox = view->xdgToplevel->getGeometry();
    grabGeoBox.moveTopLeft(view->pos + grabGeoBox.topLeft());
    resizingEdges = edges;
}

bool TinywlServer::handleKeybinding(xkb_keysym_t sym)
{
    switch (sym) {
    case XKB_KEY_Escape:
        wl_display_terminate(display);
        qApp->exit();
        break;
    case XKB_KEY_F1:
        if (views.size() < 2)
            break;

        focusView(views.at(1), views.at(1)->xdgToplevel->handle()->base->surface);
        break;
    default:
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    wlr_log_init(WLR_DEBUG, NULL);
    QGuiApplication app(argc, argv);

    app.setApplicationName("tinywl");
    app.setApplicationVersion("0.0.1");

    QCommandLineOption startup("s", "startup command", "command");
    QCommandLineParser cl;

    cl.addOption(startup);
    cl.addHelpOption();
    cl.addVersionOption();
    cl.process(app);

    TinywlServer server;
    if (!server.start())
        return -1;

    if (cl.isSet(startup)) {
        const QString command = cl.value(startup);
        QProcess::startDetached("/bin/sh", {"-c", command});
    }

    return app.exec();
}

#include "main.moc"
