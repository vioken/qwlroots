// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackend.h"

#include <wlr/backend.h>

QW_BEGIN_NAMESPACE

class QWBackendPrivate : public QWObjectPrivate
{
public:
    QWBackendPrivate(QWBackend *qq)
        : QWObjectPrivate(qq)
    {

    }
    ~QWBackendPrivate() {
        if (handle)
            wlr_backend_destroy(handle);
    }

    QW_DECLARE_PUBLIC(QWBackend)
    wlr_backend *handle = nullptr;
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
    , QWObject(*new QWBackendPrivate(this))
{
    d_func()->handle = reinterpret_cast<wlr_backend*>(handle);
}

QWBackend::~QWBackend()
{

}

clockid_t QWBackend::presentationClock() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_presentation_clock(d->handle);
}

int QWBackend::drmFd() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_drm_fd(d->handle);
}

bool QWBackend::start()
{
    Q_D(QWBackend);
    return wlr_backend_start(d->handle);
}

QW_END_NAMESPACE
