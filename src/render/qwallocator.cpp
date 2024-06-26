// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwallocator.h"
#include "qwbackend.h"
#include "qwrenderer.h"
#include "qwbuffer.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/render/allocator.h>
}

QW_BEGIN_NAMESPACE

class QWAllocatorPrivate : public QWWrapObjectPrivate
{
public:
    QWAllocatorPrivate(wlr_allocator *handle, bool isOwner, QWAllocator *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_allocator_destroy))
    {

    }

    QW_DECLARE_PUBLIC(QWAllocator)
};

QWAllocator::QWAllocator(wlr_allocator *handle, bool isOwner)
    : QWWrapObject(*new QWAllocatorPrivate(handle, isOwner, this))
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
    return static_cast<QWAllocator*>(QWAllocatorPrivate::map.value(handle));
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
