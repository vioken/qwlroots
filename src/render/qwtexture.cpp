// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

extern "C" {
#include <wlr/render/wlr_texture.h>
}

#include "qwtexture.h"
#include "qwrenderer.h"
#include "types/qwbuffer.h"

QW_BEGIN_NAMESPACE

class QWTexturePrivate : public QWObjectPrivate
{
public:
    QWTexturePrivate(void *handle, QWTexture *qq)
        : QWObjectPrivate(handle, qq)
    {

    }
    ~QWTexturePrivate() {
        Q_ASSERT(q_func()->handle());
        wlr_texture_destroy(q_func()->handle());
    }

    QW_DECLARE_PUBLIC(QWTexture)
};

QWTexture::QWTexture(wlr_texture *handle)
    : QWObject(*new QWTexturePrivate(handle, this))
{

}

QWTexture *QWTexture::fromPixels(QWRenderer *renderer, uint32_t fmt, uint32_t stride,
                                 uint32_t width, uint32_t height, const void *data)
{
    auto texture = wlr_texture_from_pixels(renderer->handle(),
                                           fmt, stride, width, height, data);
    if (!texture)
        return nullptr;
    return new QWTexture(texture);
}

QWTexture *QWTexture::fromDmabuf(QWRenderer *renderer, wlr_dmabuf_attributes *attribs)
{
    auto texture = wlr_texture_from_dmabuf(renderer->handle(), attribs);
    if (!texture)
        return nullptr;
    return new QWTexture(texture);
}

QWTexture *QWTexture::fromBuffer(QWRenderer *renderer, QWBuffer *buffer)
{
    auto texture = wlr_texture_from_buffer(renderer->handle(), buffer->handle());
    if (!texture)
        return nullptr;
    return new QWTexture(texture);
}

QWTexture::~QWTexture()
{

}

bool QWTexture::update(QWBuffer *buffer, pixman_region32 *damage)
{
    Q_D(QWTexture);
    return wlr_texture_update_from_buffer(handle(), buffer->handle(), damage);
}

QW_END_NAMESPACE
