// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/render/swapchain.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(swapchain)
{
public:
    // wlr_allocator *alloc, int width, int height, const wlr_drm_format *format
    QW_FUNC_STATIC(swapchain, create)

    // int *age
    QW_FUNC_MEMBER(swapchain, acquire)
    // wlr_buffer *buffer
    QW_FUNC_MEMBER(swapchain, set_buffer_submitted)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(swapchain, destroy)
};

QW_END_NAMESPACE
