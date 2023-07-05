// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgshell.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"

#include <QPointF>
#include <QRect>
#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_shell.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWXdgShellPrivate : public QWObjectPrivate
{
public:
    QWXdgShellPrivate(wlr_xdg_shell *handle, bool isOwner, QWXdgShell *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgShellPrivate::on_destroy);
        sc.connect(&handle->events.new_surface, this, &QWXdgShellPrivate::on_new_surface);
    }
    ~QWXdgShellPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWXdgShell(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(void *data);

    static QHash<void*, QWXdgShell*> map;
    QW_DECLARE_PUBLIC(QWXdgShell)
    QWSignalConnector sc;
};
QHash<void*, QWXdgShell*> QWXdgShellPrivate::map;

void QWXdgShellPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWXdgShellPrivate::on_new_surface(void *data)
{
    Q_EMIT q_func()->newSurface(reinterpret_cast<wlr_xdg_surface*>(data));
}

QWXdgShell::QWXdgShell(wlr_xdg_shell *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgShellPrivate(handle, isOwner, this))
{

}

QWXdgShell *QWXdgShell::create(QWDisplay *display, uint32_t version)
{
    auto handle = wlr_xdg_shell_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWXdgShell(handle, true);
}

QWXdgShell *QWXdgShell::get(wlr_xdg_shell *handle)
{
    return QWXdgShellPrivate::map.value(handle);
}

QWXdgShell *QWXdgShell::from(wlr_xdg_shell *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgShell(handle, false);
}

class QWXdgSurfacePrivate : public QWObjectPrivate
{
public:
    QWXdgSurfacePrivate(wlr_xdg_surface *handle, bool isOwner, QWXdgSurface *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgSurfacePrivate::on_destroy);
        sc.connect(&handle->events.ping_timeout, this, &QWXdgSurfacePrivate::on_ping_timeout);
        sc.connect(&handle->events.new_popup, this, &QWXdgSurfacePrivate::on_new_popup);
#if WLR_VERSION_MINOR <= 16
        sc.connect(&handle->events.map, this, &QWXdgSurfacePrivate::on_map);
        sc.connect(&handle->events.unmap, this, &QWXdgSurfacePrivate::on_unmap);
#endif
        sc.connect(&handle->events.configure, this, &QWXdgSurfacePrivate::on_configure);
        sc.connect(&handle->events.ack_configure, this, &QWXdgSurfacePrivate::on_ack_configure);
    }
    ~QWXdgSurfacePrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWXdgSurface(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_ping_timeout(void *);
    void on_new_popup(wlr_xdg_popup *data);
#if WLR_VERSION_MINOR <= 16
    void on_map(void *);
    void on_unmap(void *);
#endif
    void on_configure(void *data);
    void on_ack_configure(void *data);

    static QHash<void*, QWXdgSurface*> map;
    QW_DECLARE_PUBLIC(QWXdgSurface)
    QWSignalConnector sc;
};
QHash<void*, QWXdgSurface*> QWXdgSurfacePrivate::map;

void QWXdgSurfacePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWXdgSurfacePrivate::on_ping_timeout(void *)
{
    Q_EMIT q_func()->pingTimeout();
}

void QWXdgSurfacePrivate::on_new_popup(wlr_xdg_popup *data)
{
    Q_EMIT q_func()->newPopup(QWXdgPopup::from(data));
}

#if WLR_VERSION_MINOR <= 16
void QWXdgSurfacePrivate::on_map(void *)
{
    Q_EMIT q_func()->surface()->map();
}

void QWXdgSurfacePrivate::on_unmap(void *)
{
    Q_EMIT q_func()->surface()->unmap();
}
#endif

void QWXdgSurfacePrivate::on_configure(void *data)
{
    Q_EMIT q_func()->configure(reinterpret_cast<wlr_xdg_surface_configure*>(data));
}

void QWXdgSurfacePrivate::on_ack_configure(void *data)
{
    Q_EMIT q_func()->ackConfigure(reinterpret_cast<wlr_xdg_surface_configure*>(data));
}

QWXdgSurface::QWXdgSurface(wlr_xdg_surface *handle, bool isOwner)
    : QWXdgSurface(*new QWXdgSurfacePrivate(handle, isOwner, this))
{

}

QWXdgSurface::QWXdgSurface(QWXdgSurfacePrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{

}

QWXdgSurface *QWXdgSurface::get(wlr_xdg_surface *handle)
{
    return QWXdgSurfacePrivate::map.value(handle);
}

QWXdgSurface *QWXdgSurface::from(wl_resource *resource)
{
    auto handle = wlr_xdg_surface_from_resource(resource);
    if (handle->role == WLR_XDG_SURFACE_ROLE_POPUP)
        return QWXdgPopup::from(handle->popup);
    return QWXdgToplevel::from(handle->toplevel);
}

QWXdgSurface *QWXdgSurface::from(QWSurface *surface)
{
#if WLR_VERSION_MINOR > 16
    auto handle = wlr_xdg_surface_try_from_wlr_surface(surface->handle());
#else
    if (!wlr_surface_is_xdg_surface(surface->handle()))
        return nullptr;
    auto handle = wlr_xdg_surface_from_wlr_surface(surface->handle());
#endif
    if (!handle)
        return nullptr;

    if (handle->role == WLR_XDG_SURFACE_ROLE_POPUP)
        return QWXdgPopup::from(handle->popup);
    return QWXdgToplevel::from(handle->toplevel);
}

QWXdgPopup *QWXdgSurface::toPopup() const
{
    if (handle()->role != WLR_XDG_SURFACE_ROLE_POPUP)
        return nullptr;
    auto popup = qobject_cast<QWXdgPopup*>(const_cast<QWXdgSurface*>(this));
    Q_ASSERT(popup);
    return popup;
}

QWXdgToplevel *QWXdgSurface::topToplevel() const
{
    if (handle()->role != WLR_XDG_SURFACE_ROLE_TOPLEVEL)
        return nullptr;
    auto toplevel = qobject_cast<QWXdgToplevel*>(const_cast<QWXdgSurface*>(this));
    Q_ASSERT(toplevel);
    return toplevel;
}

QWSurface *QWXdgSurface::surface() const
{
    return QWSurface::from(handle()->surface);
}

void QWXdgSurface::ping()
{
    wlr_xdg_surface_ping(handle());
}

uint32_t QWXdgSurface::scheduleConfigure()
{
    return wlr_xdg_surface_schedule_configure(handle());
}

QWSurface *QWXdgSurface::surfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    auto* surface = wlr_xdg_surface_surface_at(handle(), xpos.x(), xpos.y(),
                                      subPos ? &subPos->rx() : nullptr,
                                      subPos ? &subPos->ry() : nullptr);
    if (!surface)
        return nullptr;
    return QWSurface::from(surface);
}

QWSurface *QWXdgSurface::popupSurfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    auto* surface = wlr_xdg_surface_popup_surface_at(handle(), xpos.x(), xpos.y(),
                                            subPos ? &subPos->rx() : nullptr,
                                            subPos ? &subPos->ry() : nullptr);
    if (!surface)
        return nullptr;
    return QWSurface::from(surface);
}

QRect QWXdgSurface::getGeometry() const
{
    wlr_box box;
    wlr_xdg_surface_get_geometry(handle(), &box);
    return QRect(box.x, box.y, box.width, box.height);
}

void QWXdgSurface::forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_xdg_surface_for_each_surface(handle(), iterator, userData);
}

void QWXdgSurface::forEachPopupSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_xdg_surface_for_each_popup_surface(handle(), iterator, userData);
}

class QWXdgPopupPrivate : public QWXdgSurfacePrivate
{
public:
    QWXdgPopupPrivate(wlr_xdg_popup *handle, bool isOwner, QWXdgPopup *qq)
        : QWXdgSurfacePrivate(handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.reposition, this, &QWXdgPopupPrivate::on_reposition);
    }
    ~QWXdgPopupPrivate () {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner) {
            wlr_xdg_popup_destroy(q_func()->handle());
            m_handle = nullptr;
        }
    }

    void on_reposition(void *);
    QW_DECLARE_PUBLIC(QWXdgPopup)
};

void QWXdgPopupPrivate::on_reposition(void *)
{
    Q_EMIT q_func()->reposition();
}

QWXdgPopup::QWXdgPopup(wlr_xdg_popup *handle, bool isOwner)
    : QWXdgSurface(*new QWXdgPopupPrivate(handle, isOwner, this))
{

}

wlr_xdg_popup *QWXdgPopup::handle() const
{
    Q_ASSERT(QWXdgSurface::handle()->role == WLR_XDG_SURFACE_ROLE_POPUP);
    return QWXdgSurface::handle()->popup;
}

QWXdgPopup *QWXdgPopup::get(wlr_xdg_popup *handle)
{
    auto surface = QWXdgSurface::get(handle->base);
    if (!surface)
        return nullptr;
    auto o = qobject_cast<QWXdgPopup*>(surface);
    Q_ASSERT(o);
    return o;
}

QWXdgPopup *QWXdgPopup::from(wlr_xdg_popup *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgPopup(handle, false);
}

QWXdgPopup *QWXdgPopup::from(wl_resource *resource)
{
    auto *handle = wlr_xdg_popup_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

QPointF QWXdgPopup::getPosition() const
{
    QPointF pos;
    wlr_xdg_popup_get_position(handle(), &pos.rx(), &pos.ry());
    return pos;
}

QPoint QWXdgPopup::getToplevelCoords(const QPoint &popupsPos) const
{
    QPoint pos;
    wlr_xdg_popup_get_toplevel_coords(handle(), popupsPos.x(), popupsPos.y(), &pos.rx(), &pos.ry());
    return pos;
}

void QWXdgPopup::unconstrainFromBox(const QRect &toplevelSpaceBox)
{
    wlr_box box {
        .x = toplevelSpaceBox.x(),
        .y = toplevelSpaceBox.y(),
        .width = toplevelSpaceBox.width(),
        .height = toplevelSpaceBox.height()
    };
    wlr_xdg_popup_unconstrain_from_box(handle(), &box);
}

class QWXdgToplevelPrivate : public QWXdgSurfacePrivate
{
public:
    QWXdgToplevelPrivate(wlr_xdg_toplevel *handle, bool isOwner, QWXdgToplevel *qq)
        : QWXdgSurfacePrivate(handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.request_maximize, this, &QWXdgToplevelPrivate::on_request_maximize);
        sc.connect(&handle->events.request_fullscreen, this, &QWXdgToplevelPrivate::on_request_fullscreen);
        sc.connect(&handle->events.request_minimize, this, &QWXdgToplevelPrivate::on_request_minimize);
        sc.connect(&handle->events.request_move, this, &QWXdgToplevelPrivate::on_request_move);
        sc.connect(&handle->events.request_resize, this, &QWXdgToplevelPrivate::on_request_resize);
        sc.connect(&handle->events.request_show_window_menu, this, &QWXdgToplevelPrivate::on_request_show_window_menu);
        sc.connect(&handle->events.set_parent, this, &QWXdgToplevelPrivate::on_set_parent);
        sc.connect(&handle->events.set_title, this, &QWXdgToplevelPrivate::on_set_title);
        sc.connect(&handle->events.set_app_id, this, &QWXdgToplevelPrivate::on_set_app_id);
    }

    void on_request_maximize(void *);
    void on_request_fullscreen(void *);

    void on_request_minimize(void *);
    void on_request_move(void *data);
    void on_request_resize(void *data);
    void on_request_show_window_menu(void *data);
    void on_set_parent(void *);
    void on_set_title(void *);
    void on_set_app_id(void *);

    QW_DECLARE_PUBLIC(QWXdgToplevel)
};

void QWXdgToplevelPrivate::on_request_maximize(void *)
{
    Q_EMIT q_func()->requestMaximize(q_func()->handle()->requested.maximized);
}

void QWXdgToplevelPrivate::on_request_fullscreen(void *)
{
    Q_EMIT q_func()->requestFullscreen(q_func()->handle()->requested.fullscreen);
}

void QWXdgToplevelPrivate::on_request_minimize(void *)
{
    Q_EMIT q_func()->requestMinimize(q_func()->handle()->requested.minimized);
}

void QWXdgToplevelPrivate::on_request_move(void *data)
{
    Q_EMIT q_func()->requestMove(reinterpret_cast<wlr_xdg_toplevel_move_event*>(data));
}

void QWXdgToplevelPrivate::on_request_resize(void *data)
{
    Q_EMIT q_func()->requestResize(reinterpret_cast<wlr_xdg_toplevel_resize_event*>(data));
}

void QWXdgToplevelPrivate::on_request_show_window_menu(void *data)
{
    Q_EMIT q_func()->requestShowWindowMenu(reinterpret_cast<wlr_xdg_toplevel_show_window_menu_event*>(data));
}

void QWXdgToplevelPrivate::on_set_parent(void *)
{
    Q_EMIT q_func()->parentChanged(QWXdgToplevel::from(q_func()->handle()->parent));
}

void QWXdgToplevelPrivate::on_set_title(void *)
{
    Q_EMIT q_func()->titleChanged(q_func()->handle()->title);
}

void QWXdgToplevelPrivate::on_set_app_id(void *)
{
    Q_EMIT q_func()->appidChanged(q_func()->handle()->app_id);
}

QWXdgToplevel::QWXdgToplevel(wlr_xdg_toplevel *handle, bool isOwner)
    : QWXdgSurface(*new QWXdgToplevelPrivate(handle, isOwner, this))
{

}

wlr_xdg_toplevel *QWXdgToplevel::handle() const
{
    Q_ASSERT(QWXdgSurface::handle()->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);
    return QWXdgSurface::handle()->toplevel;
}

QWXdgToplevel *QWXdgToplevel::get(wlr_xdg_toplevel *handle)
{
    auto surface = QWXdgSurface::get(handle->base);
    if (!surface)
        return nullptr;
    auto o = qobject_cast<QWXdgToplevel*>(surface);
    Q_ASSERT(o);
    return o;
}

QWXdgToplevel *QWXdgToplevel::from(wlr_xdg_toplevel *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgToplevel(handle, false);
}

QWXdgToplevel *QWXdgToplevel::from(wl_resource *resource)
{
    return from(wlr_xdg_toplevel_from_resource(resource));
}

uint32_t QWXdgToplevel::setSize(const QSize &size)
{
    return wlr_xdg_toplevel_set_size(handle(), size.width(), size.height());
}

uint32_t QWXdgToplevel::setActivated(bool activated)
{
    return wlr_xdg_toplevel_set_activated(handle(), activated);
}

uint32_t QWXdgToplevel::setMaximized(bool maximized)
{
    return wlr_xdg_toplevel_set_maximized(handle(), maximized);
}

uint32_t QWXdgToplevel::setFullscreen(bool fullscreen)
{
    return wlr_xdg_toplevel_set_fullscreen(handle(), fullscreen);
}

uint32_t QWXdgToplevel::setResizing(bool resizing)
{
    return wlr_xdg_toplevel_set_resizing(handle(), resizing);
}

uint32_t QWXdgToplevel::setTiled(uint32_t tiledEdges)
{
    return wlr_xdg_toplevel_set_tiled(handle(), tiledEdges);
}

uint32_t QWXdgToplevel::setBounds(const QSize &bounds)
{
    return wlr_xdg_toplevel_set_bounds(handle(), bounds.width(), bounds.height());
}

uint32_t QWXdgToplevel::setWmCapabilities(uint32_t caps)
{
    return wlr_xdg_toplevel_set_wm_capabilities(handle(), caps);
}

void QWXdgToplevel::sendClose()
{
    wlr_xdg_toplevel_send_close(handle());
}

bool QWXdgToplevel::setParent(QWXdgToplevel *parent)
{
    return wlr_xdg_toplevel_set_parent(handle(), parent->handle());
}

QW_END_NAMESPACE
