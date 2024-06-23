// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qw_global.h>

extern "C" {
#include <wlr/render/wlr_texture.h>
}

QW_BEGIN_NAMESPACE

class QWRenderer;
class QWDmabufAttributes;
class QWBuffer;

// TODO: remove
class QW_EXPORT QWTexture
{
public:
    QWTexture() = delete;
    QW_DISALLOW_DESTRUCTOR(QWTexture)

    void operator delete(QWTexture *p, std::destroying_delete_t);

    wlr_texture *handle() const;

    static QWTexture *from(wlr_texture *handle);

    static QWTexture *fromPixels(QWRenderer *renderer, uint32_t fmt, uint32_t stride,
                                 uint32_t width, uint32_t height, const void *data);
    static QWTexture *fromDmabuf(QWRenderer *renderer, wlr_dmabuf_attributes *attribs);
    static QWTexture *fromBuffer(QWRenderer *renderer, QWBuffer *buffer);

    bool update(QWBuffer *buffer, pixman_region32 *damage);
};

class QW_CLASS_REINTERPRET_CAST(texture)
{
public:
    QW_FUNC_STATIC(texture, from_pixels)
    QW_FUNC_STATIC(texture, from_dmabuf)
    QW_FUNC_STATIC(texture, from_buffer)

    QW_FUNC_MEMBER(texture, update_from_buffer)
    QW_FUNC_MEMBER(texture, destroy)
};

QW_END_NAMESPACE
