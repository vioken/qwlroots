// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct pixman_region32;
struct wlr_texture;
struct wlr_dmabuf_attributes;

QW_BEGIN_NAMESPACE

class QWRenderer;
class QWDmabufAttributes;
class QWBuffer;
class QW_EXPORT QWTexture
{
public:
    QWTexture() = delete;
    ~QWTexture() = delete;

    void operator delete(QWTexture *p, std::destroying_delete_t);

    wlr_texture *handle() const;

    static QWTexture *from(wlr_texture *handle);

    static QWTexture *fromPixels(QWRenderer *renderer, uint32_t fmt, uint32_t stride,
                                 uint32_t width, uint32_t height, const void *data);
    static QWTexture *fromDmabuf(QWRenderer *renderer, wlr_dmabuf_attributes *attribs);
    static QWTexture *fromBuffer(QWRenderer *renderer, QWBuffer *buffer);

    bool update(QWBuffer *buffer, pixman_region32 *damage);
};

QW_END_NAMESPACE
