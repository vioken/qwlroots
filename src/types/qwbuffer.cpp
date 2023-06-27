// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbuffer.h"
#include "render/qwrenderer.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

class QWBufferPrivate : public QWObjectPrivate
{
public:
    QWBufferPrivate(wlr_buffer *handle, bool isOwner, QWBuffer *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWBufferPrivate::on_destroy);
        sc.connect(&handle->events.release, this, &QWBufferPrivate::on_release);
    }
    ~QWBufferPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_buffer_drop(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_release(void *);

    static QHash<void*, QWBuffer*> map;
    QW_DECLARE_PUBLIC(QWBuffer)
    QWSignalConnector sc;
};
QHash<void*, QWBuffer*> QWBufferPrivate::map;

void QWBufferPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWBufferPrivate::on_release(void *)
{
    Q_EMIT q_func()->release();
}

QWBuffer *QWBuffer::get(wlr_buffer *handle)
{
    return QWBufferPrivate::map.value(handle);
}

QWBuffer *QWBuffer::from(wlr_buffer *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWBuffer(handle, false);
}

QWBuffer *QWBuffer::from(wl_resource *resource)
{
    auto handle = wlr_buffer_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

bool QWBuffer::isBuffer(wl_resource *resource)
{
    return wlr_resource_is_buffer(resource);
}

QWBuffer::QWBuffer(wlr_buffer *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWBufferPrivate(handle, isOwner, this))
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

bool QWBuffer::clientBufferApplyDamage(wlr_client_buffer *buffer, QWBuffer *next,
                                       pixman_region32 *damage)
{
    Q_D(QWBuffer);
    return wlr_client_buffer_apply_damage(buffer, next->handle(), damage);
}

QW_END_NAMESPACE
