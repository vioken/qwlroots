// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(virtual_pointer_manager_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(newVirtualPointer, wlr_virtual_pointer_v1_new_pointer_event*)

public:
    QW_FUNC_STATIC(virtual_pointer_manager_v1, create)
};

QW_END_NAMESPACE
