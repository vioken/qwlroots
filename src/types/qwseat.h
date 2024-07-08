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
    // wl_display *display, const char *name
    QW_FUNC_STATIC(seat, create)

    // wl_client *wl_client
    QW_FUNC_MEMBER(seat, client_for_wl_client)
    // wlr_keyboard *keyboard
    QW_FUNC_MEMBER(seat, set_keyboard)
    // Empty parameters
    QW_FUNC_MEMBER(seat, get_keyboard)
    // uint32_t capabilities
    QW_FUNC_MEMBER(seat, set_capabilities)
    // wlr_data_source *source, uint32_t serial
    QW_FUNC_MEMBER(seat, set_selection)
    // Empty parameters
    QW_FUNC_MEMBER(seat, keyboard_clear_focus)
    // Empty parameters
    QW_FUNC_MEMBER(seat, keyboard_end_grab)
    // wlr_surface *surface, const uint32_t keycodes[], size_t num_keycodes, const wlr_keyboard_modifiers *modifiers
    QW_FUNC_MEMBER(seat, keyboard_enter)
    // Empty parameters
    QW_FUNC_MEMBER(seat, keyboard_has_grab)
    // Empty parameters
    QW_FUNC_MEMBER(seat, keyboard_notify_clear_focus)
    // wlr_surface *surface, const uint32_t keycodes[], size_t num_keycodes, const wlr_keyboard_modifiers *modifiers
    QW_FUNC_MEMBER(seat, keyboard_notify_enter)
    // uint32_t time_msec, uint32_t key, uint32_t state
    QW_FUNC_MEMBER(seat, keyboard_notify_key)
    // const wlr_keyboard_modifiers *modifiers
    QW_FUNC_MEMBER(seat, keyboard_notify_modifiers)
    // uint32_t time_msec, uint32_t key, uint32_t state
    QW_FUNC_MEMBER(seat, keyboard_send_key)
    // const wlr_keyboard_modifiers *modifiers
    QW_FUNC_MEMBER(seat, keyboard_send_modifiers)
    // wlr_seat_keyboard_grab *grab
    QW_FUNC_MEMBER(seat, keyboard_start_grab)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_end_grab)
    // wlr_surface *surface, double sx, double sy
    QW_FUNC_MEMBER(seat, pointer_enter)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_has_grab)
    // uint32_t time_msec, enum wlr_axis_orientation orientation, double value, int32_t value_discrete, enum wlr_axis_source source
    QW_FUNC_MEMBER(seat, pointer_notify_axis)
    // uint32_t time_msec, uint32_t button, enum wlr_button_state state
    QW_FUNC_MEMBER(seat, pointer_notify_button)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_notify_clear_focus)
    // wlr_surface *surface, double sx, double sy
    QW_FUNC_MEMBER(seat, pointer_notify_enter)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_notify_frame)
    // uint32_t time_msec, double sx, double sy
    QW_FUNC_MEMBER(seat, pointer_notify_motion)
    // uint32_t time_msec, enum wlr_axis_orientation orientation, double value, int32_t value_discrete, enum wlr_axis_source source
    QW_FUNC_MEMBER(seat, pointer_send_axis)
    // uint32_t time_msec, uint32_t button, enum wlr_button_state state
    QW_FUNC_MEMBER(seat, pointer_send_button)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_send_frame)
    // uint32_t time_msec, double sx, double sy
    QW_FUNC_MEMBER(seat, pointer_send_motion)
    // wlr_seat_pointer_grab *grab
    QW_FUNC_MEMBER(seat, pointer_start_grab)
    // wlr_surface *surface
    QW_FUNC_MEMBER(seat, pointer_surface_has_focus)
    // double sx, double sy
    QW_FUNC_MEMBER(seat, pointer_warp)
    // Empty parameters
    QW_FUNC_MEMBER(seat, pointer_clear_focus)
    // const char *name
    QW_FUNC_MEMBER(seat, set_name)
    // Empty parameters
    QW_FUNC_MEMBER(seat, touch_end_grab)
    // int32_t touch_id
    QW_FUNC_MEMBER(seat, touch_get_point)
    // Empty parameters
    QW_FUNC_MEMBER(seat, touch_has_grab)
    // wlr_surface *surface
    QW_FUNC_MEMBER(seat, touch_notify_cancel)
    // wlr_surface *surface, uint32_t time_msec, int32_t touch_id, double sx, double sy
    QW_FUNC_MEMBER(seat, touch_notify_down)
    // Empty parameters
    QW_FUNC_MEMBER(seat, touch_notify_frame)
    // uint32_t time_msec, int32_t touch_id, double sx, double sy
    QW_FUNC_MEMBER(seat, touch_notify_motion)
    // uint32_t time_msec, int32_t touch_id
    QW_FUNC_MEMBER(seat, touch_notify_up)
    // Empty parameters
    QW_FUNC_MEMBER(seat, touch_num_points)
    // uint32_t time_msec, int32_t touch_id
    QW_FUNC_MEMBER(seat, touch_point_clear_focus)
    // wlr_surface *surface, uint32_t time_msec, int32_t touch_id, double sx, double sy
    QW_FUNC_MEMBER(seat, touch_point_focus)
    // wlr_surface *surface
    QW_FUNC_MEMBER(seat, touch_send_cancel)
    // wlr_surface *surface, uint32_t time_msec, int32_t touch_id, double sx, double sy
    QW_FUNC_MEMBER(seat, touch_send_down)
    // Empty parameters
    QW_FUNC_MEMBER(seat, touch_send_frame)
    // uint32_t time_msec, int32_t touch_id, double sx, double sy
    QW_FUNC_MEMBER(seat, touch_send_motion)
    // uint32_t time_msec, int32_t touch_id
    QW_FUNC_MEMBER(seat, touch_send_up)
    // wlr_seat_touch_grab *grab
    QW_FUNC_MEMBER(seat, touch_start_grab)
    // wlr_surface *origin, uint32_t serial
    QW_FUNC_MEMBER(seat, validate_pointer_grab_serial)
    // wlr_surface *origin, uint32_t serial, wlr_touch_point **point_ptr
    QW_FUNC_MEMBER(seat, validate_touch_grab_serial)
    // wlr_surface *surface
    QW_FUNC_MEMBER(surface, accepts_touch)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(seat, destroy)
};

class QW_CLASS_OBJECT(seat_client)
{
    QW_OBJECT
    Q_OBJECT

public:
    // Empty parameters
    QW_FUNC_MEMBER(seat_client, next_serial)
    // uint32_t serial
    QW_FUNC_MEMBER(seat_client, validate_event_serial)

    // wl_resource *resource
    QW_FUNC_STATIC(seat_client, from_resource)
    // wl_resource *resource
    QW_FUNC_STATIC(seat_client, from_pointer_resource)
};

QW_END_NAMESPACE
