// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xwayland)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(ready)
    QW_SIGNAL(new_surface, wlr_xwayland_surface*)
    QW_SIGNAL(remove_startup_info, wlr_xwayland_remove_startup_info_event*)

public:
    QW_FUNC_STATIC(xwayland, create)

    QW_FUNC_MEMBER(xwayland, set_cursor)
    QW_FUNC_MEMBER(xwayland, set_seat)
};

QW_END_NAMESPACE
