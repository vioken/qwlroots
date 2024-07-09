// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR
// GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <xcb/xproto.h>
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xwayland_surface)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(request_configure, wlr_xwayland_surface_configure_event*)
    QW_SIGNAL(request_move)
    QW_SIGNAL(request_resize, wlr_xwayland_resize_event*)
    QW_SIGNAL(request_minimize, wlr_xwayland_minimize_event*)
    QW_SIGNAL(request_maximize)
    QW_SIGNAL(request_fullscreen)
    QW_SIGNAL(request_activate)
    QW_SIGNAL(set_title)
    QW_SIGNAL(set_class)
    QW_SIGNAL(set_role)
    QW_SIGNAL(set_parent)
    QW_SIGNAL(set_startup_id)
    QW_SIGNAL(set_window_type)
    QW_SIGNAL(set_hints)
    QW_SIGNAL(set_decorations)
    QW_SIGNAL(set_override_redirect)
    QW_SIGNAL(set_geometry)
    QW_SIGNAL(ping_timeout)
    QW_SIGNAL(associate)
    QW_SIGNAL(dissociate)

public:
    QW_FUNC_STATIC(xwayland_surface, try_from_wlr_surface)

    QW_FUNC_MEMBER(xwayland_surface, activate)
    QW_FUNC_MEMBER(xwayland_surface, restack)
    QW_FUNC_MEMBER(xwayland_surface, configure)
    QW_FUNC_MEMBER(xwayland_surface, close)
    QW_FUNC_MEMBER(xwayland_surface, set_withdrawn)
    QW_FUNC_MEMBER(xwayland_surface, set_minimized)
    QW_FUNC_MEMBER(xwayland_surface, set_maximized)
    QW_FUNC_MEMBER(xwayland_surface, set_fullscreen)
    QW_FUNC_MEMBER(xwayland_surface, ping)
    QW_FUNC_MEMBER(xwayland, icccm_input_model)
    QW_FUNC_MEMBER(xwayland_or_surface, wants_focus)
};

QW_END_NAMESPACE
