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

public:
    QW_SIGNAL(release)

public:
    QW_FUNC_MEMBER(buffer, try_from_resource)
    QW_FUNC_MEMBER(buffer, drop)
    QW_FUNC_MEMBER(buffer, lock)
    QW_FUNC_MEMBER(buffer, unlock)
    QW_FUNC_MEMBER(buffer, get_dmabuf)
    QW_FUNC_MEMBER(buffer, get_shm)
    QW_FUNC_MEMBER(buffer, begin_data_ptr_access)
    QW_FUNC_MEMBER(buffer, end_data_ptr_access)
};

class QW_CLASS_REINTERPRET_CAST(client_buffer)
{
public:
    QW_FUNC_STATIC(client_buffer, get)
};

QW_END_NAMESPACE
