// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_pointer.h>
#define static
#include <wlr/types/wlr_compositor.h>
#undef static
}

QW_BEGIN_NAMESPACE
class QW_CLASS_OBJECT(seat)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(pointer_grab_begin)
    QW_SIGNAL(pointer_grab_end)
    QW_SIGNAL(keyboard_grab_begin)
    QW_SIGNAL(keyboard_grab_end)
    QW_SIGNAL(touch_grab_begin)
    QW_SIGNAL(touch_grab_end)
    QW_SIGNAL(request_set_cursor, wlr_seat_pointer_request_set_cursor_event*)
    QW_SIGNAL(request_set_selection, wlr_seat_request_set_selection_event*)
    QW_SIGNAL(set_selection)
    QW_SIGNAL(request_set_primary_selection, wlr_seat_request_set_primary_selection_event*)
    QW_SIGNAL(set_primary_selection)
    QW_SIGNAL(request_start_drag, wlr_seat_request_start_drag_event*)
    QW_SIGNAL(start_drag, wlr_drag*)

public:
    QW_FUNC_STATIC(seat, create)

    QW_FUNC_MEMBER(seat, client_for_wl_client)
    QW_FUNC_MEMBER(seat, set_keyboard)
    QW_FUNC_MEMBER(seat, get_keyboard)
    QW_FUNC_MEMBER(seat, set_capabilities)
    QW_FUNC_MEMBER(seat, set_selection)
    QW_FUNC_MEMBER(seat, keyboard_clear_focus)
    QW_FUNC_MEMBER(seat, keyboard_end_grab)
    QW_FUNC_MEMBER(seat, keyboard_enter)
    QW_FUNC_MEMBER(seat, keyboard_has_grab)
    QW_FUNC_MEMBER(seat, keyboard_notify_clear_focus)
    QW_FUNC_MEMBER(seat, keyboard_notify_enter)
    QW_FUNC_MEMBER(seat, keyboard_notify_key)
    QW_FUNC_MEMBER(seat, keyboard_notify_modifiers)
    QW_FUNC_MEMBER(seat, keyboard_send_key)
    QW_FUNC_MEMBER(seat, keyboard_send_modifiers)
    QW_FUNC_MEMBER(seat, keyboard_start_grab)
    QW_FUNC_MEMBER(seat, pointer_end_grab)
    QW_FUNC_MEMBER(seat, pointer_enter)
    QW_FUNC_MEMBER(seat, pointer_has_grab)
    QW_FUNC_MEMBER(seat, pointer_notify_axis)
    QW_FUNC_MEMBER(seat, pointer_notify_button)
    QW_FUNC_MEMBER(seat, pointer_notify_clear_focus)
    QW_FUNC_MEMBER(seat, pointer_notify_enter)
    QW_FUNC_MEMBER(seat, pointer_notify_frame)
    QW_FUNC_MEMBER(seat, pointer_notify_motion)
    QW_FUNC_MEMBER(seat, pointer_send_axis)
    QW_FUNC_MEMBER(seat, pointer_send_button)
    QW_FUNC_MEMBER(seat, pointer_send_frame)
    QW_FUNC_MEMBER(seat, pointer_send_motion)
    QW_FUNC_MEMBER(seat, pointer_start_grab)
    QW_FUNC_MEMBER(seat, pointer_surface_has_focus)
    QW_FUNC_MEMBER(seat, pointer_warp)
    QW_FUNC_MEMBER(seat, pointer_clear_focus)
    QW_FUNC_MEMBER(seat, set_name)
    QW_FUNC_MEMBER(seat, touch_end_grab)
    QW_FUNC_MEMBER(seat, touch_get_point)
    QW_FUNC_MEMBER(seat, touch_has_grab)
    QW_FUNC_MEMBER(seat, touch_notify_cancel)
    QW_FUNC_MEMBER(seat, touch_notify_down)
    QW_FUNC_MEMBER(seat, touch_notify_frame)
    QW_FUNC_MEMBER(seat, touch_notify_motion)
    QW_FUNC_MEMBER(seat, touch_notify_up)
    QW_FUNC_MEMBER(seat, touch_num_points)
    QW_FUNC_MEMBER(seat, touch_point_clear_focus)
    QW_FUNC_MEMBER(seat, touch_point_focus)
    QW_FUNC_MEMBER(seat, touch_send_cancel)
    QW_FUNC_MEMBER(seat, touch_send_down)
    QW_FUNC_MEMBER(seat, touch_send_frame)
    QW_FUNC_MEMBER(seat, touch_send_motion)
    QW_FUNC_MEMBER(seat, touch_send_up)
    QW_FUNC_MEMBER(seat, touch_start_grab)
    QW_FUNC_MEMBER(seat, validate_pointer_grab_serial)
    QW_FUNC_MEMBER(seat, validate_touch_grab_serial)
    QW_FUNC_MEMBER(surface, accepts_touch)

protected:
    QW_FUNC_MEMBER(seat, destroy)
};

class QW_CLASS_OBJECT(seat_client)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_MEMBER(seat_client, next_serial)
    QW_FUNC_MEMBER(seat_client, validate_event_serial)

    QW_FUNC_STATIC(seat_client, from_resource)
    QW_FUNC_STATIC(seat_client, from_pointer_resource)
};

QW_END_NAMESPACE
