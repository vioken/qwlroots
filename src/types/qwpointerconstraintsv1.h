// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(pointer_constraint_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(set_region)

public:
    QW_FUNC_MEMBER(pointer_constraint_v1, send_activated)
    QW_FUNC_MEMBER(pointer_constraint_v1, send_deactivated)
};

class QW_CLASS_OBJECT(pointer_constraints_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(new_constraint, wlr_pointer_constraint_v1 *)

public:
    QW_FUNC_STATIC(pointer_constraints_v1, create)
    QW_FUNC_MEMBER(pointer_constraints_v1, constraint_for_surface)
};

QW_END_NAMESPACE
