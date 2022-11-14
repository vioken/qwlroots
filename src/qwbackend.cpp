// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackend.h"
#include "util/qwsignalconnector.h"

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
        sc.connect(&this->handle()->events.new_input, this, &QWBackendPrivate::on_new_input);
        sc.connect(&this->handle()->events.new_input, this, &QWBackendPrivate::on_new_output);
        sc.connect(&this->handle()->events.destroy, &sc, &QWSignalConnector::invalidate);
    }
    ~QWBackendPrivate() {
        Q_ASSERT(handle());
        wlr_backend_destroy(handle());
    }

    void on_new_input(void *data);
    void on_new_output(void *data);

    inline wlr_backend *handle() const {
        return q_func()->handle<wlr_backend>();
    }

    QW_DECLARE_PUBLIC(QWBackend)
    QWSignalConnector sc;
};


void QWBackendPrivate::on_new_input(void *data)
{
    Q_Q(QWBackend);
    auto device = reinterpret_cast<wlr_input_device*>(data);
    Q_ASSERT(device);
    Q_EMIT q->newInput(device);
}

void QWBackendPrivate::on_new_output(void *data)
{
    Q_Q(QWBackend);
    auto output = reinterpret_cast<wlr_output*>(data);
    Q_ASSERT(output);
    Q_EMIT q->newOutput(output);
}

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
