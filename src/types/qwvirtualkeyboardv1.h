// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(virtual_keyboard_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_virtual_keyboard, wlr_virtual_keyboard_v1*)

public:
    QW_FUNC_STATIC(virtual_keyboard_manager_v1, create, wlr_virtual_keyboard_manager_v1*, wl_display *display)
};

class QW_CLASS_OBJECT(virtual_keyboard_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(input_device, get_virtual_keyboard, wlr_virtual_keyboard_v1 *, wlr_input_device *wlr_dev)
};

QW_END_NAMESPACE

