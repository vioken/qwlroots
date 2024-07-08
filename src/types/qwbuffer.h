// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(buffer)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(release)

public:
    // wl_resource *resource
    QW_FUNC_STATIC(buffer, try_from_resource)

    // Empty parameters
    QW_FUNC_MEMBER(buffer, drop)
    // Empty parameters
    QW_FUNC_MEMBER(buffer, lock)
    // Empty parameters
    QW_FUNC_MEMBER(buffer, unlock)
    // wlr_dmabuf_attributes *attribs
    QW_FUNC_MEMBER(buffer, get_dmabuf)
    // wlr_shm_attributes *attribs
    QW_FUNC_MEMBER(buffer, get_shm)
    // uint32_t flags, void **data, uint32_t *format, size_t *stride
    QW_FUNC_MEMBER(buffer, begin_data_ptr_access)
    // Empty parameters
    QW_FUNC_MEMBER(buffer, end_data_ptr_access)
};

class QW_CLASS_REINTERPRET_CAST(client_buffer)
{
public:
    // wlr_buffer *buffer
    QW_FUNC_STATIC(client_buffer, get)
};

QW_END_NAMESPACE
