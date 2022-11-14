// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbuffer.h"
#include "render/qwrenderer.h"

extern "C" {
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

class QWBufferPrivate : public QWObjectPrivate
{
public:
    QWBufferPrivate(void *handle, QWBuffer *qq)
        : QWObjectPrivate(handle, qq)
    {

    }
    ~QWBufferPrivate() {
        wlr_buffer_drop(handle());
    }

    inline wlr_buffer *handle() const {
        return q_func()->handle<wlr_buffer>();
    }

    QW_DECLARE_PUBLIC(QWBuffer)
};

QWBuffer *QWBuffer::fromResource(wl_resource *resource, QObject *parent)
{
    auto buffer = wlr_buffer_from_resource(resource);
    if (!buffer)
        return nullptr;
    return new QWBuffer(buffer, parent);
}

bool QWBuffer::isBuffer(wl_resource *resource)
{
    return wlr_resource_is_buffer(resource);
}

QWBuffer::QWBuffer(void *handle, QObject *parent)
    : QWObject(*new QWBufferPrivate(handle, this))
    , QObject(parent)
{

}

void QWBuffer::drop()
{
    Q_D(QWBuffer);
    wlr_buffer_drop(d->handle());
}

void QWBuffer::lock()
{
    Q_D(QWBuffer);
    wlr_buffer_lock(d->handle());
}

void QWBuffer::unlock()
{
    Q_D(QWBuffer);
    wlr_buffer_unlock(d->handle());
}

bool QWBuffer::getDmabuf(wlr_dmabuf_attributes *attribs) const
{
    Q_D(const QWBuffer);
    return wlr_buffer_get_dmabuf(d->handle(), attribs);
}

bool QWBuffer::getShm(wlr_shm_attributes *attribs) const
{
    Q_D(const QWBuffer);
    return wlr_buffer_get_shm(d->handle(), attribs);
}

void QWBuffer::beginDataPtrAccess(uint32_t flags, void **data,
                                  uint32_t *format, size_t *stride)
{
    Q_D(QWBuffer);
    wlr_buffer_begin_data_ptr_access(d->handle(), flags, data, format, stride);
}

void QWBuffer::endDataPtrAccess()
{
    Q_D(QWBuffer);
    wlr_buffer_end_data_ptr_access(d->handle());
}

wlr_client_buffer *QWBuffer::clientBufferCreate(const QWRenderer *renderer)
{
    Q_D(QWBuffer);
    return wlr_client_buffer_create(d->handle(), renderer->handle<wlr_renderer>());
}

wlr_client_buffer *QWBuffer::clientBufferGet() const
{
    Q_D(const QWBuffer);
    return wlr_client_buffer_get(d->handle());
}

bool QWBuffer::clientBufferApplyDamage(wlr_client_buffer *buffer, wlr_buffer *next,
                                       pixman_region32 *damage)
{
    Q_D(QWBuffer);
    return wlr_client_buffer_apply_damage(buffer, next, damage);
}

QW_END_NAMESPACE
