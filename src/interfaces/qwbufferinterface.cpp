// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbufferinterface.h"

#include <qwbuffer.h>

extern "C" {
#include <wlr/interfaces/wlr_buffer.h>
#include <wlr/types/wlr_buffer.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_buffer : public wlr_buffer
{
    _wlr_buffer(QWBufferInterface *i)
        : interface(i)
    {
    }

    QWBufferInterface *interface;
};

inline static QWBufferInterface *interface(wlr_buffer *handle)
{
    auto _handle = static_cast<_wlr_buffer *>(handle);
    Q_ASSERT(_handle->interface && _handle->impl == handle->impl);
    return static_cast<_wlr_buffer *>(handle)->interface;
}

namespace impl {
static void destroy(wlr_buffer *handle)
{
    delete interface(handle);
}

static bool get_dmabuf(wlr_buffer *handle, wlr_dmabuf_attributes *attribs)
{
    return interface(handle)->getDmabuf(attribs);
}

static bool get_shm(wlr_buffer *handle, wlr_shm_attributes *attribs)
{
    return interface(handle)->getShm(attribs);
}

static bool begin_data_ptr_access(wlr_buffer *handle,
                                  uint32_t flags,
                                  void **data,
                                  uint32_t *format,
                                  size_t *stride)
{
    return interface(handle)->beginDataPtrAccess(flags, data, format, stride);
}

static void end_data_ptr_access(wlr_buffer *handle)
{
    return interface(handle)->endDataPtrAccess();
}

} // namespace impl

QWBufferInterface::~QWBufferInterface()
{
    free(handle());
    delete impl();
}

QWBufferInterface *QWBufferInterface::get(QWBuffer *handle)
{
    return interface(handle->handle());
}

bool QWBufferInterface::getDmabuf(wlr_dmabuf_attributes *) const
{
    return false;
}

bool QWBufferInterface::getShm(wlr_shm_attributes *) const
{
    return false;
}

bool QWBufferInterface::beginDataPtrAccess(uint32_t, void **, uint32_t *, size_t *)
{
    return false;
}

void QWBufferInterface::endDataPtrAccess()
{
}

void QWBufferInterface::init(FuncMagicKey funMagicKey, int width, int height)
{
    auto impl = new wlr_buffer_impl{
        .destroy = impl::destroy,
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_dmabuf, &QWBufferInterface::getDmabuf),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_shm, &QWBufferInterface::getShm),
        QW_INIT_INTERFACE_FUNC(funMagicKey,
                               begin_data_ptr_access,
                               &QWBufferInterface::beginDataPtrAccess),
        QW_INIT_INTERFACE_FUNC(funMagicKey,
                               end_data_ptr_access,
                               &QWBufferInterface::endDataPtrAccess),
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_buffer));
    static_cast<_wlr_buffer *>(m_handle)->interface = this;
    wlr_buffer_init(handle(), impl, width, height);
}

QW_END_NAMESPACE
