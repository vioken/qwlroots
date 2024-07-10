// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_fractional_scale_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(fractional_scale_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(fractional_scale_v1, notify_scale, void, wlr_surface *surface, double scale)
    QW_FUNC_STATIC(fractional_scale_manager_v1, create, qw_fractional_scale_manager_v1 *, wl_display *display, uint32_t version)
};

QW_END_NAMESPACE

