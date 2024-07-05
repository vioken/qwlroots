// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_gamma_control_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(gamma_control_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(gamma_control_manager_v1, create)
    QW_FUNC_MEMBER(gamma_control_manager_v1, get_control)
};

class QW_CLASS_REINTERPRET_CAST(gamma_control_v1)
{
public:
    QW_FUNC_MEMBER(gamma_control_v1, apply)
    QW_FUNC_MEMBER(gamma_control_v1, send_failed_and_destroy)
};

QW_END_NAMESPACE
