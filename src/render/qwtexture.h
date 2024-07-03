// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/render/wlr_texture.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(texture)
{
public:
    QW_FUNC_STATIC(texture, from_pixels)
    QW_FUNC_STATIC(texture, from_dmabuf)
    QW_FUNC_STATIC(texture, from_buffer)

    QW_FUNC_MEMBER(texture, update_from_buffer)

protected:
    QW_FUNC_MEMBER(texture, destroy)
};

QW_END_NAMESPACE
