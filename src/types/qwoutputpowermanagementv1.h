// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_output_power_management_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(output_power_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(set_mode, wlr_output_power_v1_set_mode_event*)

public:
    QW_FUNC_STATIC(output_power_manager_v1, create, qw_output_power_manager_v1 *, wl_display *display)
};

QW_END_NAMESPACE
