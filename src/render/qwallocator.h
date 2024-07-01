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
    QW_FUNC_MEMBER(allocator, autocreate)
    QW_FUNC_MEMBER(allocator, create_buffer)

private:
    QW_FUNC_MEMBER(allocator, destroy)
};

QW_END_NAMESPACE
