// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>
#include <qtmetamacros.h>

extern "C" {
#include <wlr/types/wlr_output_power_management_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(output_power_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(set_mode, wlr_output_power_v1_set_mode_event*)

public:
    QW_FUNC_STATIC(output_power_manager_v1, create)
    QW_FUNC_MEMBER(output_power_manager_v1, send_failed)
    QW_FUNC_MEMBER(output_power_manager_v1, send_succeeded)
};

QW_END_NAMESPACE
