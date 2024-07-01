// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(output_configuration_v1)
{
public:
    QW_FUNC_STATIC(output_configuration_v1, create)
    QW_FUNC_MEMBER(output_configuration_v1, send_failed)
    QW_FUNC_MEMBER(output_configuration_v1, send_succeeded)
};

class QW_CLASS_REINTERPRET_CAST(output_configuration_head_v1)
{
public:
    QW_FUNC_STATIC(output_configuration_head_v1, create)
};

class QW_CLASS_OBJECT(output_manager_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(apply, wlr_output_configuration_v1 *)
    QW_SIGNAL(test, wlr_output_configuration_v1 *)

public:
    QW_FUNC_STATIC(output_manager_v1, create)
    QW_FUNC_MEMBER(output_manager_v1, set_configuration)
};

QW_END_NAMESPACE
