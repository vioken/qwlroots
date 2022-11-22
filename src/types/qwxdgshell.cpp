// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgshell.h"
#include "util/qwsignalconnector.h"

#include <QPointF>
#include <QRect>

extern "C" {
#include <wlr/types/wlr_xdg_shell.h>
}

QW_BEGIN_NAMESPACE

class QWXdgShellPrivate : public QWObjectPrivate
{
public:
    QWXdgShellPrivate(void *handle, QWXdgShell *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.destroy, this, &QWXdgShellPrivate::on_destroy);
        sc.connect(&qq->handle()->events.new_surface, this, &QWXdgShellPrivate::on_new_surface);
    }
    ~QWXdgShellPrivate() {
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(void *data);

    QW_DECLARE_PUBLIC(QWXdgShell)
    QWSignalConnector sc;
};

void QWXdgShellPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

void QWXdgShellPrivate::on_new_surface(void *data)
{
    Q_EMIT q_func()->newSurface(reinterpret_cast<wlr_xdg_surface*>(data));
}

QWXdgShell::QWXdgShell(wlr_xdg_shell *handle)
    : QObject(nullptr)
    , QWObject(*new QWXdgShellPrivate(handle, this))
{

}

QWXdgShell *QWXdgShell::create(wl_display *display, uint32_t version)
{
    auto handle = wlr_xdg_shell_create(display, version);
    if (!handle)
        return nullptr;
    return new QWXdgShell(handle);
}

class QWXdgSurfacePrivate : public QWObjectPrivate
{
public:
    QWXdgSurfacePrivate(void *handle, QWXdgSurface *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.destroy, this, &QWXdgSurfacePrivate::on_destroy);
        sc.connect(&qq->handle()->events.ping_timeout, this, &QWXdgSurfacePrivate::on_ping_timeout);
        sc.connect(&qq->handle()->events.new_popup, this, &QWXdgSurfacePrivate::on_new_popup);
        sc.connect(&qq->handle()->events.map, this, &QWXdgSurfacePrivate::on_map);
        sc.connect(&qq->handle()->events.unmap, this, &QWXdgSurfacePrivate::on_unmap);
        sc.connect(&qq->handle()->events.configure, this, &QWXdgSurfacePrivate::on_configure);
        sc.connect(&qq->handle()->events.ack_configure, this, &QWXdgSurfacePrivate::on_ack_configure);
    }
    ~QWXdgSurfacePrivate() {
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_ping_timeout(void *);
    void on_new_popup(void *data);
    void on_map(void *);
    void on_unmap(void *);
    void on_configure(void *data);
    void on_ack_configure(void *data);

    QW_DECLARE_PUBLIC(QWXdgSurface)
    QWSignalConnector sc;
};

void QWXdgSurfacePrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

void QWXdgSurfacePrivate::on_ping_timeout(void *)
{
    Q_EMIT q_func()->pingTimeout();
}

void QWXdgSurfacePrivate::on_new_popup(void *data)
{
    Q_EMIT q_func()->newPopup(reinterpret_cast<wlr_xdg_popup*>(data));
}

void QWXdgSurfacePrivate::on_map(void *)
{
    Q_EMIT q_func()->map();
}

void QWXdgSurfacePrivate::on_unmap(void *)
{
    Q_EMIT q_func()->unmap();
}

void QWXdgSurfacePrivate::on_configure(void *data)
{
    Q_EMIT q_func()->configure(reinterpret_cast<wlr_xdg_surface_configure*>(data));
}

void QWXdgSurfacePrivate::on_ack_configure(void *data)
{
    Q_EMIT q_func()->ackConfigure(reinterpret_cast<wlr_xdg_surface_configure*>(data));
}

QWXdgSurface::QWXdgSurface(wlr_xdg_surface *handle)
    : QWXdgSurface(*new QWXdgSurfacePrivate(handle, this))
{

}

QWXdgSurface::QWXdgSurface(QWXdgSurfacePrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{

}

wlr_xdg_surface *QWXdgSurface::from(wl_resource *resource)
{
    return wlr_xdg_surface_from_resource(resource);
}

wlr_xdg_surface *QWXdgSurface::from(wlr_surface *surface)
{
    return wlr_xdg_surface_from_wlr_surface(surface);
}

void QWXdgSurface::ping()
{
    wlr_xdg_surface_ping(handle());
}

uint32_t QWXdgSurface::scheduleConfigure()
{
    return wlr_xdg_surface_schedule_configure(handle());
}

wlr_surface *QWXdgSurface::surfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    return wlr_xdg_surface_surface_at(handle(), xpos.x(), xpos.y(),
                                      subPos ? &subPos->rx() : nullptr,
                                      subPos ? &subPos->ry() : nullptr);
}

wlr_surface *QWXdgSurface::popupSurfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    return wlr_xdg_surface_popup_surface_at(handle(), xpos.x(), xpos.y(),
                                            subPos ? &subPos->rx() : nullptr,
                                            subPos ? &subPos->ry() : nullptr);
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
    QWXdgPopupPrivate(void *handle, QWXdgPopup *qq)
        : QWXdgSurfacePrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.reposition, this, &QWXdgPopupPrivate::on_reposition);
    }
    ~QWXdgPopupPrivate () {
        sc.invalidate();
        if (m_handle)
            wlr_xdg_popup_destroy(q_func()->handle());
    }

    void on_reposition(void *);
    QW_DECLARE_PUBLIC(QWXdgPopup)
};

void QWXdgPopupPrivate::on_reposition(void *)
{
    Q_EMIT q_func()->reposition();
}

QWXdgPopup::QWXdgPopup(wlr_xdg_popup *handle)
    : QWXdgSurface(*new QWXdgPopupPrivate(handle->base, this))
{

}

wlr_xdg_popup *QWXdgPopup::from(wl_resource *resource)
{
    return wlr_xdg_popup_from_resource(resource);
}

wlr_xdg_popup *QWXdgPopup::handle() const
{
    Q_ASSERT(QWXdgSurface::handle()->role == WLR_XDG_SURFACE_ROLE_POPUP);
    return QWXdgSurface::handle()->popup;
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
    QWXdgToplevelPrivate(void *handle, QWXdgToplevel *qq)
        : QWXdgSurfacePrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.request_maximize, this, &QWXdgToplevelPrivate::on_request_maximize);
        sc.connect(&qq->handle()->events.request_fullscreen, this, &QWXdgToplevelPrivate::on_request_fullscreen);
        sc.connect(&qq->handle()->events.request_minimize, this, &QWXdgToplevelPrivate::on_request_minimize);
        sc.connect(&qq->handle()->events.request_move, this, &QWXdgToplevelPrivate::on_request_move);
        sc.connect(&qq->handle()->events.request_resize, this, &QWXdgToplevelPrivate::on_request_resize);
        sc.connect(&qq->handle()->events.request_show_window_menu, this, &QWXdgToplevelPrivate::on_request_show_window_menu);
        sc.connect(&qq->handle()->events.set_parent, this, &QWXdgToplevelPrivate::on_set_parent);
        sc.connect(&qq->handle()->events.set_title, this, &QWXdgToplevelPrivate::on_set_title);
        sc.connect(&qq->handle()->events.set_app_id, this, &QWXdgToplevelPrivate::on_set_app_id);
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
    Q_EMIT q_func()->parentChanged(q_func()->handle()->parent);
}

void QWXdgToplevelPrivate::on_set_title(void *)
{
    Q_EMIT q_func()->titleChanged(q_func()->handle()->title);
}

void QWXdgToplevelPrivate::on_set_app_id(void *)
{
    Q_EMIT q_func()->appidChanged(q_func()->handle()->app_id);
}

QWXdgToplevel::QWXdgToplevel(wlr_xdg_toplevel *handle)
    : QWXdgSurface(*new QWXdgToplevelPrivate(handle->base, this))
{

}

wlr_xdg_toplevel *QWXdgToplevel::from(wl_resource *resource)
{
    return wlr_xdg_toplevel_from_resource(resource);
}

wlr_xdg_toplevel *QWXdgToplevel::handle() const
{
    Q_ASSERT(QWXdgSurface::handle()->role == WLR_XDG_SURFACE_ROLE_TOPLEVEL);
    return QWXdgSurface::handle()->toplevel;
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

bool QWXdgToplevel::setParsent(wlr_xdg_toplevel *parent)
{
    return wlr_xdg_toplevel_set_parent(handle(), parent);
}

QW_END_NAMESPACE
