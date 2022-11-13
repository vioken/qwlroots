// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackend.h"

extern "C" {
#include <wlr/backend.h>
}

QW_BEGIN_NAMESPACE

class QWBackendPrivate : public QWObjectPrivate
{
public:
    QWBackendPrivate(void *handle, QWBackend *qq)
        : QWObjectPrivate(handle, qq)
    {

    }
    ~QWBackendPrivate() {
        Q_ASSERT(handle());
        wlr_backend_destroy(handle());
    }

    inline wlr_backend *handle() const {
        return q_func()->handle<wlr_backend>();
    }

    QW_DECLARE_PUBLIC(QWBackend)
};

QWBackend *QWBackend::autoCreate(wl_display *display, QObject *parent)
{
    auto handle = wlr_backend_autocreate(display);
    if (!handle)
        return nullptr;
    return new QWBackend(handle, parent);
}

QWBackend::QWBackend(void *handle, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWBackendPrivate(handle, this))
{

}

QWBackend::~QWBackend()
{

}

clockid_t QWBackend::presentationClock() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_presentation_clock(d->handle());
}

int QWBackend::drmFd() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_drm_fd(d->handle());
}

bool QWBackend::start()
{
    Q_D(QWBackend);
    return wlr_backend_start(d->handle());
}

QW_END_NAMESPACE
