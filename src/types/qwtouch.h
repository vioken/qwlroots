// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_touch.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(touch)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(down, wlr_touch_down_event*)
    QW_SIGNAL(up, wlr_touch_up_event*)
    QW_SIGNAL(motion, wlr_touch_motion_event*)
    QW_SIGNAL(cancel, wlr_touch_cancel_event*)
    QW_SIGNAL(frame, void*)

public:
    QW_FUNC_STATIC(touch, from_input_device)
};

QW_END_NAMESPACE
