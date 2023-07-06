// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbuffer.h"
#include "qwrenderer.h"

extern "C" {
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

wlr_client_buffer *QWClientBuffer::handle() const
{
    return reinterpret_cast<wlr_client_buffer*>(const_cast<QWClientBuffer*>(this));
}

QWClientBuffer *QWClientBuffer::from(wlr_client_buffer *handle)
{
    return reinterpret_cast<QWClientBuffer*>(handle);
}

QWClientBuffer *QWClientBuffer::get(QWBuffer *buffer)
{
    auto *handle = wlr_client_buffer_get(buffer->handle());
    return from(handle);
}

QWClientBuffer *QWClientBuffer::create(QWBuffer *buffer, QWRenderer *renderer)
{
    auto *handle = wlr_client_buffer_create(buffer->handle(), renderer->handle());
    return from(handle);
}

#if WLR_VERSION_MINOR > 16
bool QWClientBuffer::applyDamage(QWBuffer *next, const pixman_region32_t *damage)
{
    return wlr_client_buffer_apply_damage(handle(), next->handle(), damage);
}
#endif

QW_END_NAMESPACE
