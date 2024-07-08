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
    // wl_display *display, uint32_t version
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
    // wl_resource *resource
    QW_FUNC_STATIC(xdg_surface, from_resource)

    // double sx, double sy, double *sub_x, double *sub_y
    QW_FUNC_MEMBER(xdg_surface, surface_at)
    // double sx, double sy, double *sub_x, double *sub_y
    QW_FUNC_MEMBER(xdg_surface, popup_surface_at)
    // wlr_box *box
    QW_FUNC_MEMBER(xdg_surface, get_geometry)
    // wlr_surface_iterator_func_t iterator, void *user_data
    QW_FUNC_MEMBER(xdg_surface, for_each_surface)
    // wlr_surface_iterator_func_t iterator, void *user_data
    QW_FUNC_MEMBER(xdg_surface, for_each_popup_surface)
    // Empty parameters
    QW_FUNC_MEMBER(xdg_surface, schedule_configure)
};

class QW_CLASS_OBJECT(xdg_popup)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(reposition)

public:
    // wl_resource *resource
    QW_FUNC_STATIC(xdg_popup, from_resource)

    // double *popup_sx, double *popup_sy
    QW_FUNC_MEMBER(xdg_popup, get_position)
    // int popup_sx, int popup_sy, int *toplevel_sx, int *toplevel_sy
    QW_FUNC_MEMBER(xdg_popup, get_toplevel_coords)
    // const wlr_box *toplevel_space_box
    QW_FUNC_MEMBER(xdg_popup, unconstrain_from_box)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(xdg_popup, destroy)
};

class QW_CLASS_OBJECT(xdg_toplevel)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(request_maximize)
    QW_SIGNAL(request_fullscreen)
    QW_SIGNAL(request_minimize)
    QW_SIGNAL(request_move, wlr_xdg_toplevel_move_event*)
    QW_SIGNAL(request_resize, wlr_xdg_toplevel_resize_event*)
    QW_SIGNAL(request_show_window_menu, wlr_xdg_toplevel_show_window_menu_event*)
    QW_SIGNAL(set_parent, wlr_xdg_toplevel*)
    QW_SIGNAL(set_title, char*)
    QW_SIGNAL(set_app_id, char*)

public:
    // wl_resource *resource
    QW_FUNC_STATIC(xdg_toplevel, from_resource)

    // int32_t width, int32_t height
    QW_FUNC_MEMBER(xdg_toplevel, set_size)
    // bool activated
    QW_FUNC_MEMBER(xdg_toplevel, set_activated)
    // bool maximized
    QW_FUNC_MEMBER(xdg_toplevel, set_maximized)
    // bool fullscreen
    QW_FUNC_MEMBER(xdg_toplevel, set_fullscreen)
    // bool resizing
    QW_FUNC_MEMBER(xdg_toplevel, set_resizing)
    // uint32_t tiled_edges
    QW_FUNC_MEMBER(xdg_toplevel, set_tiled)
    // int32_t width, int32_t height
    QW_FUNC_MEMBER(xdg_toplevel, set_bounds)
    // uint32_t caps
    QW_FUNC_MEMBER(xdg_toplevel, set_wm_capabilities)
    // Empty parameters
    QW_FUNC_MEMBER(xdg_toplevel, send_close)
    // wlr_xdg_toplevel *parent
    QW_FUNC_MEMBER(xdg_toplevel, set_parent)
};

QW_END_NAMESPACE

