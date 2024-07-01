// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_content_type_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(content_type_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(content_type_manager_v1, create)
    QW_FUNC_MEMBER(content_type_manager_v1, get_surface_content_type)
};

QW_END_NAMESPACE
