// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_pointer_gestures_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(pointer_gestures_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(pointer_gestures_v1, create)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_swipe_begin)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_swipe_update)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_swipe_end)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_pinch_begin)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_pinch_update)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_pinch_end)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_hold_begin)
    QW_FUNC_MEMBER(pointer_gestures_v1, send_hold_end)
};

QW_END_NAMESPACE

