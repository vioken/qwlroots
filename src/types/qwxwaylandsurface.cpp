// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandsurface.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"

#include <QRect>
#include <QHash>

extern "C" {
#include <xcb/xproto.h>
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
}

static_assert(std::is_same_v<wlr_xwayland_icccm_input_model_t, std::underlying_type_t<enum wlr_xwayland_icccm_input_model>>);
static_assert(std::is_same_v<xcb_stack_mode, std::underlying_type_t<xcb_stack_mode_t>>);

QW_BEGIN_NAMESPACE

class QWXWaylandSurfacePrivate : public QWWrapObjectPrivate
{
public:
    QWXWaylandSurfacePrivate(wlr_xwayland_surface *handle, bool isOwner, QWXWaylandSurface *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.request_configure, this, &QWXWaylandSurfacePrivate::on_request_configure);
        sc.connect(&handle->events.request_move, this, &QWXWaylandSurfacePrivate::on_request_move);
        sc.connect(&handle->events.request_resize, this, &QWXWaylandSurfacePrivate::on_request_resize);
        sc.connect(&handle->events.request_minimize, this, &QWXWaylandSurfacePrivate::on_request_minimize);
        sc.connect(&handle->events.request_maximize, this, &QWXWaylandSurfacePrivate::on_request_maximize);
        sc.connect(&handle->events.request_fullscreen, this, &QWXWaylandSurfacePrivate::on_request_fullscreen);
        sc.connect(&handle->events.request_activate, this, &QWXWaylandSurfacePrivate::on_request_activate);
        sc.connect(&handle->events.associate, this, &QWXWaylandSurfacePrivate::on_associate);
        sc.connect(&handle->events.dissociate, this, &QWXWaylandSurfacePrivate::on_dissociate);
        sc.connect(&handle->events.set_strut_partial, this, &QWXWaylandSurfacePrivate::on_set_strut_partial);
        sc.connect(&handle->events.set_title, this, &QWXWaylandSurfacePrivate::on_set_title);
        sc.connect(&handle->events.set_class, this, &QWXWaylandSurfacePrivate::on_set_class);
        sc.connect(&handle->events.set_role, this, &QWXWaylandSurfacePrivate::on_set_role);
        sc.connect(&handle->events.set_parent, this, &QWXWaylandSurfacePrivate::on_set_parent);
        sc.connect(&handle->events.set_startup_id, this, &QWXWaylandSurfacePrivate::on_set_startup_id);
        sc.connect(&handle->events.set_window_type, this, &QWXWaylandSurfacePrivate::on_set_window_type);
        sc.connect(&handle->events.set_hints, this, &QWXWaylandSurfacePrivate::on_set_hints);
        sc.connect(&handle->events.set_decorations, this, &QWXWaylandSurfacePrivate::on_set_decorations);
        sc.connect(&handle->events.set_override_redirect, this, &QWXWaylandSurfacePrivate::on_set_override_redirect);
        sc.connect(&handle->events.set_geometry, this, &QWXWaylandSurfacePrivate::on_set_geometry);
        sc.connect(&handle->events.ping_timeout, this, &QWXWaylandSurfacePrivate::on_ping_timeout);
    }
    ~QWXWaylandSurfacePrivate() {
        if (!m_handle)
            return;
        Q_ASSERT(!isHandleOwner);
    }

    void on_request_configure(wlr_xwayland_surface_configure_event *event);
    void on_request_move(void *);
    void on_request_resize(wlr_xwayland_resize_event *event);
    void on_request_minimize(wlr_xwayland_minimize_event *event);
    void on_request_maximize(void *);
    void on_request_fullscreen(void *);
    void on_request_activate(void *);
    void on_associate(void *);
    void on_dissociate(void *);
    void on_set_strut_partial(void *);
    void on_set_title(void *);
    void on_set_class(void *);
    void on_set_role(void *);
    void on_set_parent(void *);
    void on_set_startup_id(void *);
    void on_set_window_type(void *);
    void on_set_hints(void *);
    void on_set_decorations(void *);
    void on_set_override_redirect(void *);
    void on_set_geometry(void *);
    void on_ping_timeout(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXWaylandSurface)
};
QHash<void*, QWWrapObject*> QWXWaylandSurfacePrivate::map;

void QWXWaylandSurfacePrivate::on_request_configure(wlr_xwayland_surface_configure_event *event)
{
    Q_EMIT q_func()->requestConfigure(event);
}

void QWXWaylandSurfacePrivate::on_request_move(void *)
{
    Q_EMIT q_func()->requestMove();
}

void QWXWaylandSurfacePrivate::on_request_resize(wlr_xwayland_resize_event *event)
{
    Q_EMIT q_func()->requestResize(event);
}

void QWXWaylandSurfacePrivate::on_request_minimize(wlr_xwayland_minimize_event *event)
{
    Q_EMIT q_func()->requestMinimize(event);
}

void QWXWaylandSurfacePrivate::on_request_maximize(void *)
{
    Q_EMIT q_func()->requestMaximize();
}

void QWXWaylandSurfacePrivate::on_request_fullscreen(void *)
{
    Q_EMIT q_func()->requestFullscreen();
}

void QWXWaylandSurfacePrivate::on_request_activate(void *)
{
    Q_EMIT q_func()->requestActivate();
}

void QWXWaylandSurfacePrivate::on_associate(void *)
{
    Q_EMIT q_func()->associate();
}

void QWXWaylandSurfacePrivate::on_dissociate(void *)
{
    Q_EMIT q_func()->dissociate();
}

void QWXWaylandSurfacePrivate::on_set_strut_partial(void *)
{
    Q_EMIT q_func()->strutPartialChanged();
}

void QWXWaylandSurfacePrivate::on_set_title(void *)
{
    Q_EMIT q_func()->titleChanged();
}

void QWXWaylandSurfacePrivate::on_set_class(void *)
{
    Q_EMIT q_func()->classChanged();
}

void QWXWaylandSurfacePrivate::on_set_role(void *)
{
    Q_EMIT q_func()->roleChanged();
}

void QWXWaylandSurfacePrivate::on_set_parent(void *)
{
    Q_EMIT q_func()->parentChanged();
}

void QWXWaylandSurfacePrivate::on_set_startup_id(void *)
{
    Q_EMIT q_func()->startupIdChanged();
}

void QWXWaylandSurfacePrivate::on_set_window_type(void *)
{
    Q_EMIT q_func()->windowTypeChanged();
}

void QWXWaylandSurfacePrivate::on_set_hints(void *)
{
    Q_EMIT q_func()->hintsChanged();
}

void QWXWaylandSurfacePrivate::on_set_decorations(void *)
{
    Q_EMIT q_func()->decorationsChanged();
}

void QWXWaylandSurfacePrivate::on_set_override_redirect(void *)
{
    Q_EMIT q_func()->overrideRedirectChanged();
}

void QWXWaylandSurfacePrivate::on_set_geometry(void *)
{
    Q_EMIT q_func()->geometryChanged();
}

void QWXWaylandSurfacePrivate::on_ping_timeout(void *)
{
    Q_EMIT q_func()->pingTimeout();
}

QWXWaylandSurface::QWXWaylandSurface(wlr_xwayland_surface *handle)
    : QWWrapObject(*new QWXWaylandSurfacePrivate(handle, false, this))
{

}

QWXWaylandSurface *QWXWaylandSurface::get(wlr_xwayland_surface *handle)
{
    return static_cast<QWXWaylandSurface*>(QWXWaylandSurfacePrivate::map.value(handle));
}

QWXWaylandSurface* QWXWaylandSurface::from(wlr_xwayland_surface* handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXWaylandSurface(handle);
}

wlr_xwayland_surface* QWXWaylandSurface::handle() const
{
    return QWObject::handle<wlr_xwayland_surface>();
}

void QWXWaylandSurface::activate(bool activated)
{
    wlr_xwayland_surface_activate(handle(), activated);
}

void QWXWaylandSurface::restack(QWXWaylandSurface *sibling, xcb_stack_mode mode)
{
    wlr_xwayland_surface_restack(handle(), sibling->handle(), static_cast<xcb_stack_mode_t>(mode));
}

void QWXWaylandSurface::configure(const QRect &rect)
{
    wlr_xwayland_surface_configure(handle(), rect.x(), rect.y(), rect.width(), rect.height());
}

void QWXWaylandSurface::close()
{
    wlr_xwayland_surface_close(handle());
}

void QWXWaylandSurface::setWithdrawn(bool withdrawn)
{
    wlr_xwayland_surface_set_withdrawn(handle(), withdrawn);
}

void QWXWaylandSurface::setMinimized(bool minimized)
{
    wlr_xwayland_surface_set_minimized(handle(), minimized);
}

void QWXWaylandSurface::setMaximized(bool maximized)
{
    wlr_xwayland_surface_set_maximized(handle(), maximized);
}

void QWXWaylandSurface::setFullscreen(bool fullscreen)
{
    wlr_xwayland_surface_set_fullscreen(handle(), fullscreen);
}

QWXWaylandSurface *QWXWaylandSurface::tryFromWlrSurface(QWSurface *surface)
{
    auto handle = wlr_xwayland_surface_try_from_wlr_surface(surface->handle());
    return handle ? from(handle) : nullptr;
}

void QWXWaylandSurface::ping()
{
    wlr_xwayland_surface_ping(handle());
}

bool QWXWaylandSurface::xwaylandOrSurfaceWantsFocus() const
{
    return wlr_xwayland_or_surface_wants_focus(handle());
}

wlr_xwayland_icccm_input_model_t QWXWaylandSurface::icccmInputModel() const
{
    return wlr_xwayland_icccm_input_model(handle());
}

QW_END_NAMESPACE
