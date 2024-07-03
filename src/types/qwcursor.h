// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_cursor.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(cursor)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(motion, wlr_pointer_motion_event*)
    QW_SIGNAL(motion_absolute, wlr_pointer_motion_absolute_event*)
    QW_SIGNAL(button, wlr_pointer_button_event*)
    QW_SIGNAL(axis, wlr_pointer_axis_event*)
    QW_SIGNAL(frame)
    QW_SIGNAL(swipe_begin, wlr_pointer_swipe_begin_event*)
    QW_SIGNAL(swipe_update, wlr_pointer_swipe_update_event*)
    QW_SIGNAL(swipe_end, wlr_pointer_swipe_end_event*)
    QW_SIGNAL(pinch_begin, wlr_pointer_pinch_begin_event*)
    QW_SIGNAL(pinch_update, wlr_pointer_pinch_update_event*)
    QW_SIGNAL(pinch_end, wlr_pointer_pinch_end_event*)
    QW_SIGNAL(hold_begin, wlr_pointer_hold_begin_event*)
    QW_SIGNAL(hold_end, wlr_pointer_hold_end_event*)
    QW_SIGNAL(touch_up, wlr_touch_up_event*)
    QW_SIGNAL(touch_down, wlr_touch_down_event*)
    QW_SIGNAL(touch_motion, wlr_touch_motion_event*)
    QW_SIGNAL(touch_cancel, wlr_touch_cancel_event*)
    QW_SIGNAL(touch_frame)
    QW_SIGNAL(tablet_tool_axis, wlr_tablet_tool_axis_event*)
    QW_SIGNAL(tablet_tool_proximity, wlr_tablet_tool_proximity_event*)
    QW_SIGNAL(tablet_tool_tip, wlr_tablet_tool_tip_event*)
    QW_SIGNAL(tablet_tool_button, wlr_tablet_tool_button*)

public:
    QW_FUNC_STATIC(cursor, create)

    QW_FUNC_MEMBER(cursor, warp)
    QW_FUNC_MEMBER(cursor, warp_closest)
    QW_FUNC_MEMBER(cursor, warp_absolute)
    QW_FUNC_MEMBER(cursor, move)
    QW_FUNC_MEMBER(cursor, set_buffer)
    QW_FUNC_MEMBER(cursor, set_xcursor)
    QW_FUNC_MEMBER(cursor, unset_image)
    QW_FUNC_MEMBER(cursor, set_surface)
    QW_FUNC_MEMBER(cursor, attach_input_device)
    QW_FUNC_MEMBER(cursor, detach_input_device)
    QW_FUNC_MEMBER(cursor, attach_output_layout)
    QW_FUNC_MEMBER(cursor, map_to_output)
    QW_FUNC_MEMBER(cursor, map_input_to_output)
    QW_FUNC_MEMBER(cursor, map_to_region)
    QW_FUNC_MEMBER(cursor, map_input_to_region)
    QW_FUNC_MEMBER(cursor, absolute_to_layout_coords)

protected:
    QW_FUNC_MEMBER(cursor, destroy)
};

QW_END_NAMESPACE
