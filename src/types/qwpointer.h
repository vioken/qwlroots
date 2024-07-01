// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_pointer.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(pointer)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(motion, wlr_pointer_motion_event *)
    QW_SIGNAL(motion_absolute, wlr_pointer_motion_absolute_event *)
    QW_SIGNAL(button, wlr_pointer_button_event *)
    QW_SIGNAL(axis, wlr_pointer_axis_event *)
    QW_SIGNAL(frame)
    QW_SIGNAL(swipe_begin, wlr_pointer_swipe_begin_event *)
    QW_SIGNAL(swipe_update, wlr_pointer_swipe_update_event *)
    QW_SIGNAL(swipe_end, wlr_pointer_swipe_end_event *)
    QW_SIGNAL(pinch_begin, wlr_pointer_pinch_begin_event *)
    QW_SIGNAL(pinch_update, wlr_pointer_pinch_update_event *)
    QW_SIGNAL(pinch_end, wlr_pointer_pinch_end_event *)
    QW_SIGNAL(hold_begin, wlr_pointer_hold_begin_event *)
    QW_SIGNAL(hold_end, wlr_pointer_hold_end_event *)

public:
    QW_FUNC_STATIC(pointer, from_input_device)

};

QW_END_NAMESPACE
