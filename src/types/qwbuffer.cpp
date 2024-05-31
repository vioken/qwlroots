// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbuffer.h"
#include "render/qwrenderer.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

class QWBufferPrivate : public QWWrapObjectPrivate
{
public:
    QWBufferPrivate(wlr_buffer *handle, bool isOwner, QWBuffer *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy,
                              toDestroyFunction(wlr_buffer_drop))
    {
        sc.connect(&handle->events.release, this, &QWBufferPrivate::on_release);
    }

    void on_release(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWBuffer)
};
QHash<void*, QWWrapObject*> QWBufferPrivate::map;

void QWBufferPrivate::on_release(void *)
{
    Q_EMIT q_func()->release();
}

QWBuffer *QWBuffer::get(wlr_buffer *handle)
{
    return static_cast<QWBuffer*>(QWBufferPrivate::map.value(handle));
}

QWBuffer *QWBuffer::from(wlr_buffer *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWBuffer(handle, false);
}

QWBuffer *QWBuffer::from(wl_resource *resource)
{
    auto handle = wlr_buffer_try_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

QWBuffer::QWBuffer(wlr_buffer *handle, bool isOwner)
    : QWWrapObject(*new QWBufferPrivate(handle, isOwner, this))
{

}

void QWBuffer::drop()
{
    wlr_buffer_drop(handle());
}

void QWBuffer::lock()
{
    wlr_buffer_lock(handle());
}

void QWBuffer::unlock()
{
    wlr_buffer_unlock(handle());
}

bool QWBuffer::getDmabuf(wlr_dmabuf_attributes *attribs) const
{
    return wlr_buffer_get_dmabuf(handle(), attribs);
}

bool QWBuffer::getShm(wlr_shm_attributes *attribs) const
{
    return wlr_buffer_get_shm(handle(), attribs);
}

void QWBuffer::beginDataPtrAccess(uint32_t flags, void **data,
                                  uint32_t *format, size_t *stride)
{
    wlr_buffer_begin_data_ptr_access(handle(), flags, data, format, stride);
}

void QWBuffer::endDataPtrAccess()
{
    wlr_buffer_end_data_ptr_access(handle());
}

QW_END_NAMESPACE
