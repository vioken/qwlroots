// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(tablet_manager_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wl_display *display
    QW_FUNC_STATIC(tablet_v2, create)
};

class QW_CLASS_OBJECT(tablet_v2_tablet)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wlr_tablet_manager_v2 *manager, wlr_seat *wlr_seat, wlr_input_device *wlr_device
    QW_FUNC_STATIC(tablet, create)

    // wlr_surface *surface
    QW_FUNC_MEMBER(surface, accepts_tablet_v2)
};

class QW_CLASS_OBJECT(tablet_v2_tablet_tool)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(set_cursor, wlr_tablet_v2_event_cursor*)

public:
    // wlr_tablet_manager_v2 *manager, wlr_seat *wlr_seat, wlr_tablet_tool *wlr_tool
    QW_FUNC_STATIC(tablet_tool, create)

    // wlr_tablet_v2_tablet *tablet, wlr_surface *surface
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, proximity_in)
    // Empty parameters
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, down)
    // double x, double y
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, motion)
    // double pressure
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, pressure)
    // double distance
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, distance)
    // double x, double y
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, tilt)
    // double degrees
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, rotation)
    // double position
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, slider)
    // double degrees, int32_t clicks
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, wheel)
    // Empty parameters
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, proximity_out)
    // uint32_t button, enum zwp_tablet_pad_v2_button_state state
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, button)
    // wlr_tablet_v2_tablet *tablet, wlr_surface *surface
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_proximity_in)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_down)
    // double x, double y
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_motion)
    // double pressure
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_pressure)
    // double distance
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_distance)
    // double x, double y
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_tilt)
    // double degrees
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_rotation)
    // double position
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_slider)
    // double degrees, int32_t clicks
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_wheel)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_proximity_out)
    // uint32_t button, enum zwp_tablet_pad_v2_button_state state
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_button)
    // wlr_tablet_tool_v2_grab *grab
    QW_FUNC_MEMBER(tablet_tool_v2, start_grab)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_tool_v2, end_grab)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_tool_v2, start_implicit_grab)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_tool_v2, has_implicit_grab)
};

class QW_CLASS_OBJECT(tablet_v2_tablet_pad)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(button_feedback, wlr_tablet_v2_event_feedback*)
    QW_SIGNAL(strip_feedback, wlr_tablet_v2_event_feedback*)
    QW_SIGNAL(ring_feedback, wlr_tablet_v2_event_feedback*)

public:
    // wlr_tablet_manager_v2 *manager, wlr_seat *wlr_seat, wlr_input_device *wlr_device
    QW_FUNC_STATIC(tablet_pad, create)

    // wlr_tablet_v2_tablet *tablet, wlr_surface *surface
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, enter)
    // size_t button, uint32_t time, enum zwp_tablet_pad_v2_button_state state
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, button)
    // uint32_t strip, double position, bool finger, uint32_t time
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, strip)
    // uint32_t ring, double position, bool finger, uint32_t time
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, ring)
    // wlr_surface *surface
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, leave)
    // size_t group, uint32_t mode, uint32_t time
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, mode)
    // wlr_tablet_v2_tablet *tablet, wlr_surface *surface
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_enter)
    // size_t button, uint32_t time, enum zwp_tablet_pad_v2_button_state state
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_button)
    // uint32_t strip, double position, bool finger, uint32_t time
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_strip)
    // uint32_t ring, double position, bool finger, uint32_t time
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_ring)
    // wlr_surface *surface
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_leave)
    // size_t group, uint32_t mode, uint32_t time
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_mode)
    // Empty parameters
    QW_FUNC_MEMBER(tablet_v2, end_grab)
    // wlr_tablet_pad_v2_grab *grab
    QW_FUNC_MEMBER(tablet_v2, start_grab)
};

QW_END_NAMESPACE
