// Copyright (C) 2022 zccrs zccrs@live.com.
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
class QWTexturePrivate;
class QW_EXPORT QWTexture : public QWObject
{
    QW_DECLARE_PRIVATE(QWTexture)
public:
    explicit QWTexture(wlr_texture *handle);
    ~QWTexture();

    inline wlr_texture *handle() const {
        return QWObject::handle<wlr_texture>();
    }

    static QWTexture *fromPixels(QWRenderer *renderer, uint32_t fmt, uint32_t stride,
                                 uint32_t width, uint32_t height, const void *data);
    static QWTexture *fromDmabuf(QWRenderer *renderer, wlr_dmabuf_attributes *attribs);
    static QWTexture *fromBuffer(QWRenderer *renderer, QWBuffer *buffer);

    bool update(QWBuffer *buffer, pixman_region32 *damage);
};

QW_END_NAMESPACE
