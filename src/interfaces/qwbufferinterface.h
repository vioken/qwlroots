// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

extern "C" {
#include <wlr/interfaces/wlr_buffer.h>
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

template<typename Derive>
class QW_CLASS_INTERFACE(buffer)
{
    QW_INTERFACE_INIT(buffer)

public:
    QW_INTERFACE_FUNC_STATIC(buffer, init, void, wlr_buffer *buffer, const wlr_buffer_impl *impl, int width, int height)

    QW_INTERFACE(get_dmabuf)
    QW_INTERFACE(get_shm)
    QW_INTERFACE(begin_data_ptr_access)
    QW_INTERFACE(end_data_ptr_access)
};

QW_END_NAMESPACE
