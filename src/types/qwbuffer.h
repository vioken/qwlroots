// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_buffer;
struct wlr_dmabuf_attributes;
struct wlr_shm_attributes;
struct wlr_client_buffer;
struct wl_resource;
struct pixman_region32;

QW_BEGIN_NAMESPACE

class QWRenderer;
class QWBufferPrivate;
class QW_EXPORT QWBuffer : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWBuffer)
public:
    inline wlr_buffer *handle() const {
        return QWObject::handle<wlr_buffer>();
    }

    static QWBuffer *get(wlr_buffer *handle);
    static QWBuffer *from(wlr_buffer *handle);

    static QWBuffer *from(wl_resource *resource);
    static bool isBuffer(wl_resource *resource);

    void drop();
    void lock();
    void unlock();

    bool getDmabuf(wlr_dmabuf_attributes *attribs) const;
    bool getShm(wlr_shm_attributes *attribs) const;

    void beginDataPtrAccess(uint32_t flags, void **data, uint32_t *format, size_t *stride);
    void endDataPtrAccess();

    wlr_client_buffer *clientBufferCreate(const QWRenderer *renderer);
    wlr_client_buffer *clientBufferGet() const;

    bool clientBufferApplyDamage(wlr_client_buffer *buffer, wlr_buffer *next,
                                 pixman_region32 *damage);

Q_SIGNALS:
    void release();

private:
    QWBuffer(wlr_buffer *handle, bool isOwner);
    ~QWBuffer() = default;
};

QW_END_NAMESPACE
