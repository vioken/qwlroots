// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/render/allocator.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(allocator)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wlr_backend *backend, wlr_renderer *renderer
    QW_FUNC_STATIC(allocator, autocreate)

    // int width, int height, const wlr_drm_format *format
    QW_FUNC_MEMBER(allocator, create_buffer)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(allocator, destroy)
};

QW_END_NAMESPACE
