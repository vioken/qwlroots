// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include "math.h"
#define static
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(foreign_toplevel_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wl_display *display
    QW_FUNC_STATIC(foreign_toplevel_manager_v1, create)
};

class QW_CLASS_OBJECT(foreign_toplevel_handle_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(request_maximize, wlr_foreign_toplevel_handle_v1_maximized_event*)
    QW_SIGNAL(request_minimize, wlr_foreign_toplevel_handle_v1_minimized_event*)
    QW_SIGNAL(request_activate, wlr_foreign_toplevel_handle_v1_activated_event*)
    QW_SIGNAL(request_fullscreen, wlr_foreign_toplevel_handle_v1_fullscreen_event*)
    QW_SIGNAL(request_close)
    QW_SIGNAL(set_rectangle, wlr_foreign_toplevel_handle_v1_set_rectangle_event*)

public:
    // wlr_foreign_toplevel_manager_v1 *manager
    QW_FUNC_STATIC(foreign_toplevel_handle_v1, create)

    // const char *title
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_title)
    // const char *app_id
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_app_id)
    // wlr_output *output
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, output_enter)
    // wlr_output *output
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, output_leave)
    // bool maximized
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_maximized)
    // bool minimized
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_minimized)
    // bool activated
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_activated)
    // bool fullscreen
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_fullscreen)
    // wlr_foreign_toplevel_handle_v1 *parent
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, set_parent)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(foreign_toplevel_handle_v1, destroy)
};

QW_END_NAMESPACE

