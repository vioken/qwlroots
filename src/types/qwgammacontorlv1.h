// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include<qwobject.h>

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(gamma_control_manager_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(gammaChanged, wlr_gamma_control_manager_v1_set_gamma_event*)

public:
    QW_FUNC_STATIC(gamma_control_manager_v1, create)
};

QW_END_NAMESPACE
