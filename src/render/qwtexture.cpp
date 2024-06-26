// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtexture.h"
#include "qwrenderer.h"
#include "types/qwbuffer.h"

#include <QHash>

extern "C" {
#include <wlr/render/wlr_texture.h>
}

QW_BEGIN_NAMESPACE

void QWTexture::operator delete(QWTexture *p, std::destroying_delete_t)
{
    wlr_texture_destroy(p->handle());
}

wlr_texture *QWTexture::handle() const
{
    return reinterpret_cast<wlr_texture*>(const_cast<QWTexture*>(this));
}

QWTexture *QWTexture::from(wlr_texture *handle)
{
    return reinterpret_cast<QWTexture*>(handle);
}

QWTexture *QWTexture::fromPixels(QWRenderer *renderer, uint32_t fmt, uint32_t stride,
                                 uint32_t width, uint32_t height, const void *data)
{
    auto texture = wlr_texture_from_pixels(renderer->handle(),
                                           fmt, stride, width, height, data);
    return from(texture);
}

QWTexture *QWTexture::fromDmabuf(QWRenderer *renderer, wlr_dmabuf_attributes *attribs)
{
    auto texture = wlr_texture_from_dmabuf(renderer->handle(), attribs);
    return from(texture);
}

QWTexture *QWTexture::fromBuffer(QWRenderer *renderer, QWBuffer *buffer)
{
    auto texture = wlr_texture_from_buffer(renderer->handle(), buffer->handle());
    return from(texture);
}

bool QWTexture::update(QWBuffer *buffer, pixman_region32 *damage)
{
    return wlr_texture_update_from_buffer(handle(), buffer->handle(), damage);
}

QW_END_NAMESPACE
