// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_idle.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(idle)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(activity_notify)

public:
    QW_FUNC_STATIC(idle, create)

    QW_FUNC_MEMBER(idle, notify_activity)
    QW_FUNC_MEMBER(idle, set_enabled)
};

class QW_CLASS_OBJECT(idle_timeout)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(idle)
    QW_SIGNAL(resume)

public:
    QW_FUNC_STATIC(idle_timeout, create)

    QW_FUNC_MEMBER(idle_timeout, destroy)
};

QW_END_NAMESPACE
