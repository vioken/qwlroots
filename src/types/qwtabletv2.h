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
    QW_FUNC_STATIC(tablet_v2, create)
};

class QW_CLASS_OBJECT(tablet_v2_tablet)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(tablet, create)
    QW_FUNC_MEMBER(surface, accepts_tablet_v2)
};

class QW_CLASS_OBJECT(tablet_v2_tablet_tool)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(set_cursor, wlr_tablet_v2_event_cursor*)

public:
    QW_FUNC_STATIC(tablet_tool, create)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, proximity_in)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, down)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, motion)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, pressure)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, distance)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, tilt)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, rotation)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, slider)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, wheel)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, proximity_out)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_tool, button)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_proximity_in)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_down)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_motion)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_pressure)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_distance)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_tilt)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_rotation)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_slider)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_wheel)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_proximity_out)
    QW_FUNC_MEMBER(tablet_v2_tablet_tool, notify_button)
    QW_FUNC_MEMBER(tablet_tool_v2, start_grab)
    QW_FUNC_MEMBER(tablet_tool_v2, end_grab)
    QW_FUNC_MEMBER(tablet_tool_v2, start_implicit_grab)
    QW_FUNC_MEMBER(tablet_tool_v2, has_implicit_grab)
};

class QW_CLASS_OBJECT(tablet_v2_tablet_pad)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(button_feedback, wlr_tablet_v2_event_feedback*)
    QW_SIGNAL(strip_feedback, wlr_tablet_v2_event_feedback*)
    QW_SIGNAL(ring_feedback, wlr_tablet_v2_event_feedback*)

public:
    QW_FUNC_STATIC(tablet_pad, create)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, enter)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, button)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, strip)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, ring)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, leave)
    QW_FUNC_MEMBER(send_tablet_v2_tablet_pad, mode)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_enter)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_button)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_strip)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_ring)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_leave)
    QW_FUNC_MEMBER(tablet_v2_tablet_pad, notify_mode)
    QW_FUNC_MEMBER(tablet_v2, end_grab)
    QW_FUNC_MEMBER(tablet_v2, start_grab)
};

QW_END_NAMESPACE
