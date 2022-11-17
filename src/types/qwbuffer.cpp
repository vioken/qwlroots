// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbuffer.h"
#include "render/qwrenderer.h"
#include "util/qwsignalconnector.h"

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
        sc.connect(&q_func()->handle()->events.destroy, this, &QWBufferPrivate::on_destroy);
        sc.connect(&q_func()->handle()->events.release, this, &QWBufferPrivate::on_release);
    }
    ~QWBufferPrivate() {
        sc.invalidate();
        if (m_handle)
            wlr_buffer_drop(q_func()->handle());
    }

    void on_destroy(void *);
    void on_release(void *);

    QW_DECLARE_PUBLIC(QWBuffer)
    QWSignalConnector sc;
};

void QWBufferPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

void QWBufferPrivate::on_release(void *)
{
    Q_EMIT q_func()->release();
}

QWBuffer *QWBuffer::fromResource(wl_resource *resource)
{
    auto buffer = wlr_buffer_from_resource(resource);
    if (!buffer)
        return nullptr;
    return new QWBuffer(buffer);
}

bool QWBuffer::isBuffer(wl_resource *resource)
{
    return wlr_resource_is_buffer(resource);
}

QWBuffer::QWBuffer(wlr_buffer *handle)
    : QWObject(*new QWBufferPrivate(handle, this))
    , QObject(nullptr)
{

}

void QWBuffer::drop()
{
    Q_D(QWBuffer);
    wlr_buffer_drop(handle());
}

void QWBuffer::lock()
{
    Q_D(QWBuffer);
    wlr_buffer_lock(handle());
}

void QWBuffer::unlock()
{
    Q_D(QWBuffer);
    wlr_buffer_unlock(handle());
}

bool QWBuffer::getDmabuf(wlr_dmabuf_attributes *attribs) const
{
    Q_D(const QWBuffer);
    return wlr_buffer_get_dmabuf(handle(), attribs);
}

bool QWBuffer::getShm(wlr_shm_attributes *attribs) const
{
    Q_D(const QWBuffer);
    return wlr_buffer_get_shm(handle(), attribs);
}

void QWBuffer::beginDataPtrAccess(uint32_t flags, void **data,
                                  uint32_t *format, size_t *stride)
{
    Q_D(QWBuffer);
    wlr_buffer_begin_data_ptr_access(handle(), flags, data, format, stride);
}

void QWBuffer::endDataPtrAccess()
{
    Q_D(QWBuffer);
    wlr_buffer_end_data_ptr_access(handle());
}

wlr_client_buffer *QWBuffer::clientBufferCreate(const QWRenderer *renderer)
{
    Q_D(QWBuffer);
    return wlr_client_buffer_create(handle(), renderer->handle());
}

wlr_client_buffer *QWBuffer::clientBufferGet() const
{
    Q_D(const QWBuffer);
    return wlr_client_buffer_get(handle());
}

bool QWBuffer::clientBufferApplyDamage(wlr_client_buffer *buffer, wlr_buffer *next,
                                       pixman_region32 *damage)
{
    Q_D(QWBuffer);
    return wlr_client_buffer_apply_damage(buffer, next, damage);
}

QW_END_NAMESPACE
