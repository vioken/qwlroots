// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwallocator.h"
#include "qwbackend.h"
#include "qwrenderer.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/render/allocator.h>
}

QW_BEGIN_NAMESPACE

class QWAllocatorPrivate : public QWObjectPrivate
{
public:
    QWAllocatorPrivate(wlr_allocator *handle, QWAllocator *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&handle->events.destroy, this, &QWAllocatorPrivate::on_destroy);
    }
    ~QWAllocatorPrivate() {
        sc.invalidate();
        if (m_handle)
            wlr_allocator_destroy(q_func()->handle());
    }

    void on_destroy(void *);

    QW_DECLARE_PUBLIC(QWAllocator)
    QWSignalConnector sc;
};

void QWAllocatorPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

QWAllocator::QWAllocator(wlr_allocator *handle)
    : QObject(nullptr)
    , QWObject(*new QWAllocatorPrivate(handle, this))
{

}

QWAllocator *QWAllocator::autoCreate(QWBackend *backend, QWRenderer *renderer)
{
    auto handle = wlr_allocator_autocreate(backend->handle(), renderer->handle());
    if (!handle)
        return nullptr;
    return new QWAllocator(handle);
}

wlr_buffer *QWAllocator::createBuffer(int width, int height, const wlr_drm_format *format)
{
    return wlr_allocator_create_buffer(handle(), width, height, format);
}

QW_END_NAMESPACE
