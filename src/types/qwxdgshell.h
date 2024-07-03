// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_shell.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xdg_shell)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_surface, wlr_xdg_surface*)
#if WLR_VERSION_MINOR >= 18
    QW_SIGNAL(new_toplevel, wlr_xdg_toplevel*)
    QW_SIGNAL(new_popup, wlr_xdg_popup*)
#endif

public:
    QW_FUNC_STATIC(xdg_shell, create)
};

class QW_CLASS_OBJECT(xdg_surface)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(ping_timeout)
    QW_SIGNAL(new_popup, wlr_xdg_popup*)
    QW_SIGNAL(configure, wlr_xdg_surface_configure*)
    QW_SIGNAL(ack_configure, wlr_xdg_surface_configure*)

public:
    QW_FUNC_MEMBER(xdg_surface, surface_at)
    QW_FUNC_MEMBER(xdg_surface, popup_surface_at)
    QW_FUNC_MEMBER(xdg_surface, get_geometry)
    QW_FUNC_MEMBER(xdg_surface, for_each_surface)
    QW_FUNC_MEMBER(xdg_surface, for_each_popup_surface)
    QW_FUNC_MEMBER(xdg_surface, schedule_configure)
};

class QW_CLASS_OBJECT(xdg_popup)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(reposition)

public:
    QW_FUNC_MEMBER(xdg_popup, get_position)
    QW_FUNC_MEMBER(xdg_popup, get_toplevel_coords)
    QW_FUNC_MEMBER(xdg_popup, unconstrain_from_box)

protected:
    QW_FUNC_STATIC(xdg_popup, destroy)
};

class QW_CLASS_OBJECT(xdg_toplevel)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(request_maximize, bool)
    QW_SIGNAL(request_fullscreen, bool)
    QW_SIGNAL(request_minimize, bool)
    QW_SIGNAL(request_move, wlr_xdg_toplevel_move_event *)
    QW_SIGNAL(request_resize, wlr_xdg_toplevel_resize_event *)
    QW_SIGNAL(request_show_window_menu, wlr_xdg_toplevel_show_window_menu_event *)
    QW_SIGNAL(set_parent, wlr_xdg_toplevel *)
    QW_SIGNAL(set_title, char *)
    QW_SIGNAL(set_app_id, char *)

public:
    QW_FUNC_MEMBER(xdg_toplevel, set_size)
    QW_FUNC_MEMBER(xdg_toplevel, set_activated)
    QW_FUNC_MEMBER(xdg_toplevel, set_maximized)
    QW_FUNC_MEMBER(xdg_toplevel, set_fullscreen)
    QW_FUNC_MEMBER(xdg_toplevel, set_resizing)
    QW_FUNC_MEMBER(xdg_toplevel, set_tiled)
    QW_FUNC_MEMBER(xdg_toplevel, set_bounds)
    QW_FUNC_MEMBER(xdg_toplevel, set_wm_capabilities)
    QW_FUNC_MEMBER(xdg_toplevel, send_close)
    QW_FUNC_MEMBER(xdg_toplevel, set_parent)
};

QW_END_NAMESPACE

