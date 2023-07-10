// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_buffer;
struct wlr_buffer_impl;
struct wlr_dmabuf_attributes;
struct wlr_shm_attributes;

QW_BEGIN_NAMESPACE

class QWBuffer;
class QWBufferInterface : public QWInterface
{
    friend class QWBuffer;
public:
    virtual ~QWBufferInterface();

    virtual bool getDmabuf(wlr_dmabuf_attributes *attribs) const;
    virtual bool getShm(wlr_shm_attributes *attribs) const;
    virtual bool beginDataPtrAccess(uint32_t flags, void **data, uint32_t *format, size_t *stride);
    virtual void endDataPtrAccess();

    inline wlr_buffer *handle() const { return QWInterface::handle<wlr_buffer>(); }
    inline wlr_buffer_impl *impl() const { return QWInterface::handle<wlr_buffer_impl>(); }
    static QWBufferInterface *get(QWBuffer *handle);

protected:
    template<class T>
    inline void init(int width, int height)
    {
        init(getFuncMagicKey<T>(&T::getDmabuf,
                                &T::getShm,
                                &T::beginDataPtrAccess,
                                &T::endDataPtrAccess),
             width,
             height);
    }

    virtual void init(FuncMagicKey funMagicKey, int width, int height);
};

QW_END_NAMESPACE
