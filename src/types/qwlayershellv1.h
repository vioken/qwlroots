// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define namespace scope
#define static
#include <wlr/types/wlr_layer_shell_v1.h>
#undef namespace
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(layer_surface_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(new_popup, xdg_popup*)

public:
    QW_FUNC_STATIC(layer_surface_v1, create)
    QW_FUNC_STATIC(layer_surface_v1, try_from_wlr_surface)

    QW_FUNC_MEMBER(layer_surface_v1, destroy)
    QW_FUNC_MEMBER(layer_surface_v1, for_each_surface)
    QW_FUNC_MEMBER(layer_surface_v1, for_each_popup_surface)
    QW_FUNC_MEMBER(layer_surface_v1, at)
    QW_FUNC_MEMBER(layer_surface_v1, popup_surface_at)
};

class QW_CLASS_OBJECT(layer_shell_v1)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(new_surface, qw_layer_surface_v1 *surface)

public:
    QW_FUNC_STATIC(layer_shell_v1, create)
};

QW_END_NAMESPACE
