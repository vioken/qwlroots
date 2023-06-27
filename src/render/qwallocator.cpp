// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwallocator.h"
#include "qwbackend.h"
#include "qwrenderer.h"
#include "util/qwsignalconnector.h"
#include "qwbuffer.h"

#include <QHash>

extern "C" {
#include <wlr/render/allocator.h>
}

QW_BEGIN_NAMESPACE

class QWAllocatorPrivate : public QWObjectPrivate
{
public:
    QWAllocatorPrivate(wlr_allocator *handle, bool isOwner, QWAllocator *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWAllocatorPrivate::on_destroy);
    }
    ~QWAllocatorPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_allocator_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWAllocator*> map;
    QW_DECLARE_PUBLIC(QWAllocator)
    QWSignalConnector sc;
};
QHash<void*, QWAllocator*> QWAllocatorPrivate::map;

void QWAllocatorPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWAllocator::QWAllocator(wlr_allocator *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWAllocatorPrivate(handle, isOwner, this))
{

}

QWAllocator *QWAllocator::autoCreate(QWBackend *backend, QWRenderer *renderer)
{
    auto handle = wlr_allocator_autocreate(backend->handle(), renderer->handle());
    if (!handle)
        return nullptr;
    return new QWAllocator(handle, true);
}

QWAllocator *QWAllocator::get(wlr_allocator *handle)
{
    return QWAllocatorPrivate::map.value(handle);
}

QWAllocator *QWAllocator::from(wlr_allocator *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWAllocator(handle, false);
}

QWBuffer *QWAllocator::createBuffer(int width, int height, const wlr_drm_format *format)
{
    return QWBuffer::from(wlr_allocator_create_buffer(handle(), width, height, format));
}

QW_END_NAMESPACE
