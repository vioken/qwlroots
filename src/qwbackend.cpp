// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackend.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"
#include "types/qwoutput.h"

#include <QHash>

extern "C" {
#include <wlr/backend.h>
}

QW_BEGIN_NAMESPACE

class QWBackendPrivate : public QWObjectPrivate
{
public:
    QWBackendPrivate(wlr_backend *handle, bool isOwner, QWBackend *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_input, this, &QWBackendPrivate::on_new_input);
        sc.connect(&handle->events.new_output, this, &QWBackendPrivate::on_new_output);
        sc.connect(&handle->events.destroy, this, &QWBackendPrivate::on_destroy);
    }
    ~QWBackendPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_backend_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_new_input(wlr_input_device *device);
    void on_new_output(wlr_output *output);
    void on_destroy(void *);

    static QHash<void*, QWBackend*> map;
    QW_DECLARE_PUBLIC(QWBackend)
    QWSignalConnector sc;
};
QHash<void*, QWBackend*> QWBackendPrivate::map;

void QWBackendPrivate::on_new_input(wlr_input_device *device)
{
    Q_Q(QWBackend);
    Q_ASSERT(device);
    Q_EMIT q->newInput(device);
}

void QWBackendPrivate::on_new_output(wlr_output *output)
{
    Q_Q(QWBackend);
    Q_ASSERT(output);
    Q_EMIT q->newOutput(QWOutput::from(output));
}

void QWBackendPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWBackend *QWBackend::get(wlr_backend *handle)
{
    return QWBackendPrivate::map.value(handle);
}

QWBackend *QWBackend::from(wlr_backend *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWBackend(handle, false);
}

QWBackend *QWBackend::autoCreate(QWDisplay *display, QObject *parent)
{
#if WLR_VERSION_MINOR > 16
    auto handle = wlr_backend_autocreate(display->handle(), nullptr);
#else
    auto handle = wlr_backend_autocreate(display->handle());
#endif
    if (!handle)
        return nullptr;
    return new QWBackend(handle, true, parent);
}

QWBackend::QWBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWBackendPrivate(handle, isOwner, this))
{

}

QWBackend::~QWBackend()
{

}

clockid_t QWBackend::presentationClock() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_presentation_clock(handle());
}

int QWBackend::drmFd() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_drm_fd(handle());
}

bool QWBackend::start()
{
    Q_D(QWBackend);
    return wlr_backend_start(handle());
}

QW_END_NAMESPACE
