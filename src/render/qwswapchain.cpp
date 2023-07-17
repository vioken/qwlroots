// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwswapchain.h"

#include "qwallocator.h"
#include "qwbuffer.h"

#include <QSize>

extern "C" {
#include <wlr/render/drm_format_set.h>
#include <wlr/render/swapchain.h>
}

QW_BEGIN_NAMESPACE

void QWSwapchain::operator delete(QWSwapchain *p, std::destroying_delete_t)
{
    wlr_swapchain_destroy(p->handle());
}

QWSwapchain *QWSwapchain::create(QWAllocator *alloc, QSize size, const wlr_drm_format *format)
{
    auto* pointer = wlr_swapchain_create(alloc->handle(), size.width(), size.height(), format);
    return pointer ? from(pointer) : nullptr;
}

QWSwapchain *QWSwapchain::from(wlr_swapchain* swapchain)
{
    return reinterpret_cast<QWSwapchain*>(swapchain);
}

wlr_swapchain* QWSwapchain::handle() const
{
    return reinterpret_cast<wlr_swapchain*>(const_cast<QWSwapchain*>(this));
}

QWBuffer *QWSwapchain::acquire(int *age)
{
    auto *buffer = wlr_swapchain_acquire(handle(), age);
    return buffer ? QWBuffer::from(buffer) : nullptr;
}

void QWSwapchain::setBufferSubmitted(QWBuffer *buffer)
{
    wlr_swapchain_set_buffer_submitted(handle(), buffer->handle());
}

QW_END_NAMESPACE
