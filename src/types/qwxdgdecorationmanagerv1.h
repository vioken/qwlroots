// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_xdg_decoration_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(xdg_decoration_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_toplevel_decoration, wlr_xdg_toplevel_decoration_v1*)

public:
    QW_FUNC_STATIC(xdg_decoration_manager_v1, create)
};

class QW_CLASS_OBJECT(xdg_toplevel_decoration_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(request_mode)

public:
    QW_FUNC_MEMBER(xdg_toplevel_decoration_v1, set_mode)
};

QW_END_NAMESPACE
