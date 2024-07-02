// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xdg_decoration_manager_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_FUNC_STATIC(xdg_decoration_manager_v1, create)

public:
    QW_SIGNAL(new_toplevel_decoration, qw_xdg_toplevel_decoration_v1*)
};

class QW_CLASS_OBJECT(xdg_toplevel_decoration_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_FUNC_MEMBER(xdg_toplevel_decoration_v1, set_mode)

public:
    QW_SIGNAL(request_mode)
};

QW_END_NAMESPACE
