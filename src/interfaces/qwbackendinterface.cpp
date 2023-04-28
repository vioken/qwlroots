// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackendinterface.h"

extern "C" {
#include <wlr/backend/interface.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_backend : public wlr_backend
{
    _wlr_backend(QWBackendInterface *i)
        : interface(i) {}

    QWBackendInterface *interface;
};

inline static QWBackendInterface *interface(wlr_backend *handle) {
    auto _handle = static_cast<_wlr_backend*>(handle);
    Q_ASSERT(_handle->interface && _handle->impl == handle->impl);
    return _handle->interface;
}

namespace impl {
static bool start(wlr_backend *handle) {
    return interface(handle)->start();
}

static void destroy(wlr_backend *handle) {
    delete interface(handle);
}

static clockid_t get_presentation_clock(wlr_backend *handle) {
    return interface(handle)->getPresentationClock();
}

static int get_drm_fd(wlr_backend *handle) {
    return interface(handle)->getPresentationClock();
}

static uint32_t get_buffer_caps(wlr_backend *handle) {
    return interface(handle)->getBufferCaps();
}
}

QWBackendInterface::~QWBackendInterface()
{
    wlr_backend_finish(handle());
    free(handle());
    delete impl();
}

clockid_t QWBackendInterface::getPresentationClock() const
{
    return CLOCK_MONOTONIC;
}

int QWBackendInterface::getDrmFd() const
{
    return -1;
}

int QWBackendInterface::getBufferCaps() const
{
    return 0;
}

QWBackendInterface *QWBackendInterface::get(wlr_backend *handle)
{
    return interface(handle);
}

void QWBackendInterface::init(FuncMagicKey funMagicKey)
{
    auto impl = new wlr_backend_impl {
        .start = impl::start,
        .destroy = impl::destroy,
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_presentation_clock, &QWBackendInterface::getPresentationClock),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_drm_fd, &QWBackendInterface::getDrmFd),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_buffer_caps, &QWBackendInterface::getBufferCaps),
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_backend));
    static_cast<_wlr_backend *>(m_handle)->interface = this;
    wlr_backend_init(handle(), impl);
}

QW_END_NAMESPACE
