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


QW_END_NAMESPACE
