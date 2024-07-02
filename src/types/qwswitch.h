// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWSWITCH_H
#define QWSWITCH_H

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_switch.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(switch)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(toggle, wlr_switch_toggle_event*)

public:
    QW_FUNC_STATIC(switch, from_input_device)
};

QW_END_NAMESPACE

#endif // QWSWITCH_H
