// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandsurface.h"
#include "qwcompositor.h"

#include <QRect>

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

QWXWaylandSurface* QWXWaylandSurface::from(wlr_xwayland_surface* surface)
{
    return reinterpret_cast<QWXWaylandSurface*>(surface);
}

wlr_xwayland_surface* QWXWaylandSurface::handle() const
{
    return reinterpret_cast<wlr_xwayland_surface*>(const_cast<QWXWaylandSurface*>(this));
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

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
void QWXWaylandSurface::setWithdrawn(bool withdrawn)
{
    wlr_xwayland_surface_set_withdrawn(handle(), withdrawn);
}
#endif

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

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
QWXWaylandSurface *QWXWaylandSurface::tryFromWlrSurface(QWSurface *surface)
{
    return from(wlr_xwayland_surface_try_from_wlr_surface(surface->handle()));
}
#endif

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
