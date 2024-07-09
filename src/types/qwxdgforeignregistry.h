// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_xdg_foreign_registry.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xdg_foreign_registry)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(xdg_foreign_registry, create)

    QW_FUNC_MEMBER(xdg_foreign_registry, find_by_handle)
};

class QW_CLASS_OBJECT(xdg_foreign_exported)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_MEMBER(xdg_foreign_exported, init)
    QW_FUNC_MEMBER(xdg_foreign_exported, finish)
};

QW_END_NAMESPACE
