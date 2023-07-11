// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QProcess>
#include <QRect>
#include <QLoggingCategory>

#include <qwbackend.h>
#include <qwdisplay.h>
#include <qwrenderer.h>
#include <qwallocator.h>
#include <qwcompositor.h>
#include <qwsubcompositor.h>
#include <qwdatadevice.h>
#include <qwoutputlayout.h>
#include <qwoutput.h>
#include <qwscene.h>
#include <qwseat.h>
#include <qwxdgshell.h>
#include <qwcursor.h>
#include <qwxcursormanager.h>
#include <qwinputdevice.h>
#include <qwkeyboard.h>
#include <qwpointer.h>

#define WLR_USE_UNSTABLE
extern "C" {
// avoid replace static
#include <wayland-server-core.h>
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
    void onNewOutput(QWOutput *output);
    void onNewXdgSurface(wlr_xdg_surface *surface);
    void onMap();
    void onUnmap();
    void onXdgToplevelRequestMove(wlr_xdg_toplevel_move_event *);
    void onXdgToplevelRequestResize(wlr_xdg_toplevel_resize_event *event);
    void onXdgToplevelRequestMaximize(bool maximize);
    void onXdgToplevelRequestRequestFullscreen(bool fullscreen);

    void onCursorMotion(wlr_pointer_motion_event *event);
    void onCursorMotionAbsolute(wlr_pointer_motion_absolute_event *event);
    void onCursorButton(wlr_pointer_button_event *event);
    void onCursorAxis(wlr_pointer_axis_event *event);
    void onCursorFrame();

    void onNewInput(QWInputDevice *device);
    void onRequestSetCursor(wlr_seat_pointer_request_set_cursor_event *event);
    void onRequestSetSelection(wlr_seat_request_set_selection_event *event);

    void onKeyboardModifiers();
    void onKeyboardKey(wlr_keyboard_key_event *event);
    void onKeyboardDestroy();

    void onOutputFrame();

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

    QWDisplay *display;
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

    QList<QWKeyboard*> keyboards;
    QWSeat *seat;
};

TinywlServer::TinywlServer()
{
    display = new QWDisplay(this);
    backend = QWBackend::autoCreate(display, this);
    if (!backend)
        qFatal("failed to create wlr_backend");

    // Add output
//    auto multiBackend = qobject_cast<QWMultiBackend*>(backend);
//    multiBackend->forEachBackend([] (struct wlr_backend *backend, void *data) {
//        auto x11 = QWX11Backend::from(backend);
//        if (x11)
//            x11->createOutput();
//    }, nullptr);

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

    outputLayout = new QWOutputLayout(this);
    connect(backend, &QWBackend::newOutput, this, &TinywlServer::onNewOutput);

    scene = new QWScene(this);
    scene->attachOutputLayout(outputLayout);
    xdgShell = QWXdgShell::create(display, 3);
    connect(xdgShell, &QWXdgShell::newSurface, this, &TinywlServer::onNewXdgSurface);

    cursor = new QWCursor(this);
    cursor->attachOutputLayout(outputLayout);
    cursorManager = QWXCursorManager::create(nullptr, 24);
    cursorManager->load(1);
    connect(cursor, &QWCursor::motion, this, &TinywlServer::onCursorMotion);
    connect(cursor, &QWCursor::motionAbsolute, this, &TinywlServer::onCursorMotionAbsolute);
    connect(cursor, &QWCursor::button, this, &TinywlServer::onCursorButton);
    connect(cursor, &QWCursor::axis, this, &TinywlServer::onCursorAxis);
    connect(cursor, &QWCursor::frame, this, &TinywlServer::onCursorFrame);

    connect(backend, &QWBackend::newInput, this, &TinywlServer::onNewInput);

    seat = QWSeat::create(display, "seat0");
    connect(seat, &QWSeat::requestSetCursor, this, &TinywlServer::onRequestSetCursor);
    connect(seat, &QWSeat::requestSetSelection, this, &TinywlServer::onRequestSetSelection);
}

TinywlServer::~TinywlServer()
{
    delete backend;
}

bool TinywlServer::start()
{
    const char *socket = display->addSocketAuto();
    if (!socket) {
        return false;
    }

    if (!backend->start())
        return false;

    qputenv("WAYLAND_DISPLAY", QByteArray(socket));
    qInfo("Running Wayland compositor on WAYLAND_DISPLAY=%s", socket);

    display->start(qApp->thread());
    return true;
}

void TinywlServer::onNewOutput(QWOutput *output)
{
    Q_ASSERT(output);
    outputs.append(output);

    output->initRender(allocator, renderer);
    if (!wl_list_empty(&output->handle()->modes)) {
        auto *mode = output->preferredMode();
        output->setMode(mode);
    }

    output->enable(true);
    if (!output->commit())
        return;

    connect(output, &QWOutput::frame, this, &TinywlServer::onOutputFrame);
    outputLayout->addAuto(output);
}

void TinywlServer::onNewXdgSurface(wlr_xdg_surface *surface)
{
    if (surface->role == WLR_XDG_SURFACE_ROLE_POPUP) {
        auto *s = QWXdgPopup::from(surface->popup);
        auto parent = QWXdgSurface::from(QWSurface::from(surface->popup->parent));
        QWSceneTree *parentTree = reinterpret_cast<QWSceneTree*>(parent->handle()->data);
        surface->data = QWScene::xdgSurfaceCreate(parentTree, s);
        return;
    }
    Q_ASSERT(surface->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);

    auto view = new View();
    view->server = this;
    auto s = QWXdgToplevel::from(surface->toplevel);
    view->xdgToplevel = s;
    view->sceneTree = QWScene::xdgSurfaceCreate(scene, s);
    view->sceneTree->handle()->node.data = view;
    surface->data = view->sceneTree;
    auto *ss = s->surface();
    connect(ss, &QWSurface::map, this, &TinywlServer::onMap);
    connect(ss, &QWSurface::unmap, this, &TinywlServer::onUnmap);
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

void TinywlServer::onMap()
{
    auto surface = QWXdgSurface::from(qobject_cast<QWSurface*>(sender()));
    if (!surface)
        return;
    auto view = getView(surface);
    Q_ASSERT(view);
    views.append(view);
    focusView(view, surface->handle()->surface);
}

void TinywlServer::onUnmap()
{
    auto surface = QWXdgSurface::from(qobject_cast<QWSurface*>(sender()));
    if (!surface)
        return;
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
    cursor->move(QWPointer::from(event->pointer), QPointF(event->delta_x, event->delta_y));
    processCursorMotion(event->time_msec);
}

void TinywlServer::onCursorMotionAbsolute(wlr_pointer_motion_absolute_event *event)
{
    cursor->warpAbsolute(QWPointer::from(event->pointer), QPointF(event->x, event->y));
    processCursorMotion(event->time_msec);
}

void TinywlServer::onCursorButton(wlr_pointer_button_event *event)
{
    seat->pointerNotifyButton(event->time_msec, event->button, event->state);
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
    seat->pointerNotifyAxis(event->time_msec, event->orientation,
                                 event->delta, event->delta_discrete, event->source);
}

void TinywlServer::onCursorFrame()
{
    seat->pointerNotifyFrame();
}

void TinywlServer::onNewInput(QWInputDevice *device)
{
    if (QWKeyboard *keyboard = qobject_cast<QWKeyboard*>(device)) {

        xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
        xkb_keymap *keymap = xkb_keymap_new_from_names(context, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);

        keyboard->setKeymap(keymap);
        xkb_keymap_unref(keymap);
        xkb_context_unref(context);
        keyboard->setRepeatInfo(25, 600);

        connect(keyboard, &QWKeyboard::modifiers, this, &TinywlServer::onKeyboardModifiers);
        connect(keyboard, &QWKeyboard::key, this, &TinywlServer::onKeyboardKey);
        connect(keyboard, &QWKeyboard::destroyed, this, &TinywlServer::onKeyboardDestroy);

        seat->setKeyboard(keyboard);

        Q_ASSERT(!keyboards.contains(keyboard));
        keyboards.append(keyboard);
    } else if (device->handle()->type == WLR_INPUT_DEVICE_POINTER) {
        cursor->attachInputDevice(device);
    }

    uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
    if (!keyboards.isEmpty()) {
        caps |= WL_SEAT_CAPABILITY_KEYBOARD;
    }
    seat->setCapabilities(caps);
}

void TinywlServer::onRequestSetCursor(wlr_seat_pointer_request_set_cursor_event *event)
{
    if (seat->handle()->pointer_state.focused_client == event->seat_client)
        cursor->setSurface(QWSurface::from(event->surface), QPoint(event->hotspot_x, event->hotspot_y));
}

void TinywlServer::onRequestSetSelection(wlr_seat_request_set_selection_event *event)
{
    seat->setSelection(event->source, event->serial);
}

void TinywlServer::onKeyboardModifiers()
{
    QWKeyboard *keyboard = qobject_cast<QWKeyboard*>(QObject::sender());
    seat->setKeyboard(keyboard);
    seat->keyboardNotifyModifiers(&keyboard->handle()->modifiers);
}

void TinywlServer::onKeyboardKey(wlr_keyboard_key_event *event)
{
    QWKeyboard *keyboard = qobject_cast<QWKeyboard*>(QObject::sender());
    /* Translate libinput keycode -> xkbcommon */
    uint32_t keycode = event->keycode + 8;
    const xkb_keysym_t *syms;
    int nsyms = xkb_state_key_get_syms(keyboard->handle()->xkb_state, keycode, &syms);

    bool handled = false;
    uint32_t modifiers = keyboard->getModifiers();
    if ((modifiers & (WLR_MODIFIER_ALT | WLR_MODIFIER_CTRL))
            && event->state == WL_KEYBOARD_KEY_STATE_PRESSED) {
        for (int i = 0; i < nsyms; i++)
            handled = handleKeybinding(syms[i]);
    }

    if (!handled) {
        seat->setKeyboard(keyboard);
        seat->keyboardNotifyKey(event->time_msec, event->keycode, event->state);
    }
}

void TinywlServer::onKeyboardDestroy()
{
    QWKeyboard *keyboard = qobject_cast<QWKeyboard*>(QObject::sender());
    keyboards.removeOne(keyboard);
}

void TinywlServer::onOutputFrame()
{
    auto output = qobject_cast<QWOutput*>(sender());
    Q_ASSERT(output);
    auto sceneOutput = QWSceneOutput::from(scene, output);
#if WLR_VERSION_MINOR > 16
    sceneOutput->commit(nullptr);
#else
    sceneOutput->commit();
#endif
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    sceneOutput->sendFrameDone(&now);
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
    auto *sceneBuffer = QWSceneBuffer::from(node);
#if WLR_VERSION_MINOR > 16
    auto *sceneSurface = wlr_scene_surface_try_from_buffer(sceneBuffer->handle());
#else
    auto *sceneSurface = wlr_scene_surface_from_buffer(sceneBuffer->handle());
#endif
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
#if WLR_VERSION_MINOR > 16
    if (!view)
        cursor->setXCursor(cursorManager, "left_ptr");
#else
    if (!view)
        cursorManager->setCursor("left_ptr", cursor);
#endif
    if (surface) {
        seat->pointerNotifyEnter(QWSurface::from(surface), spos.x(), spos.y());
        seat->pointerNotifyMotion(time, spos.x(), spos.y());
    } else {
        seat->pointerClearFocus();
    }
}

void TinywlServer::focusView(View *view, wlr_surface *surface)
{
    if (!view)
        return;

    wlr_surface *prevSurface = seat->handle()->keyboard_state.focused_surface;
    if (prevSurface == surface) {
        return;
    }
    if (prevSurface) {
        auto previous = QWXdgSurface::from(QWSurface::from(prevSurface));
        auto toplevel = qobject_cast<QWXdgToplevel*>(previous);
        Q_ASSERT(toplevel);
        toplevel->setActivated(false);
    }

    view->sceneTree->raiseToTop();
    views.move(views.indexOf(view), 0);
    view->xdgToplevel->setActivated(true);

    if (QWKeyboard *keyboard = seat->getKeyboard()) {
        seat->keyboardNotifyEnter(QWSurface::from(view->xdgToplevel->handle()->base->surface),
                                       keyboard->handle()->keycodes, keyboard->handle()->num_keycodes, &keyboard->handle()->modifiers);
    }
}

void TinywlServer::beginInteractive(View *view, CursorState state, uint32_t edges)
{
    wlr_surface *focusedSurface = seat->handle()->pointer_state.focused_surface;
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
        display->terminate();
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
