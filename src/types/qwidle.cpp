// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwidle.h"
#include "qwseat.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_idle.h>
}

QW_BEGIN_NAMESPACE

/// QWIdle
class QWIdlePrivate : public QWObjectPrivate
{
public:
    QWIdlePrivate(wlr_idle *handle, bool isOwner, QWIdle *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWIdlePrivate::on_destroy);
        sc.connect(&handle->events.activity_notify, this, &QWIdlePrivate::on_activity_notify);
    }
    ~QWIdlePrivate() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_activity_notify(void *);

    static QHash<void*, QWIdle*> map;
    QW_DECLARE_PUBLIC(QWIdle)
    QWSignalConnector sc;
};
QHash<void*, QWIdle*> QWIdlePrivate::map;

void QWIdlePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWIdlePrivate::on_activity_notify(void *)
{
    Q_EMIT q_func()->activityNotify();
}

QWIdle::QWIdle(wlr_idle *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWIdlePrivate(handle, isOwner, this))
{

}

QWIdle *QWIdle::get(wlr_idle *handle)
{
    return QWIdlePrivate::map.value(handle);
}

QWIdle *QWIdle::from(wlr_idle *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWIdle(handle, false);
}

QWIdle *QWIdle::create(QWDisplay *display)
{
    auto *handle = wlr_idle_create(display->handle());
    if (!handle)
        return nullptr;
    return from(handle);
}

void QWIdle::notifyActivity(QWSeat *seat)
{
    wlr_idle_notify_activity(handle(), seat->handle());
}

void QWIdle::setEnabled(QWSeat *seat, bool enabled)
{
    wlr_idle_set_enabled(handle(), seat->handle(), enabled);
}

/// QWIdleTimeout
class QWIdleTimeoutPrivate : public QWObjectPrivate
{
public:
    QWIdleTimeoutPrivate(wlr_idle_timeout *handle, bool isOwner, QWIdleTimeout *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWIdleTimeoutPrivate::on_destroy);
        sc.connect(&handle->events.idle, this, &QWIdleTimeoutPrivate::on_idle);
        sc.connect(&handle->events.resume, this, &QWIdleTimeoutPrivate::on_resume);
    }
    ~QWIdleTimeoutPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_idle_timeout_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_idle(void *);
    void on_resume(void *);

    static QHash<void*, QWIdleTimeout*> map;
    QW_DECLARE_PUBLIC(QWIdleTimeout)
    QWSignalConnector sc;
};
QHash<void*, QWIdleTimeout*> QWIdleTimeoutPrivate::map;

void QWIdleTimeoutPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWIdleTimeoutPrivate::on_idle(void *)
{
    Q_EMIT q_func()->idle();
}

void QWIdleTimeoutPrivate::on_resume(void *)
{
    Q_EMIT q_func()->resume();
}

QWIdleTimeout::QWIdleTimeout(wlr_idle_timeout *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWIdleTimeoutPrivate(handle, isOwner, this))
{

}

QWIdleTimeout *QWIdleTimeout::get(wlr_idle_timeout *handle)
{
    return QWIdleTimeoutPrivate::map.value(handle);
}

QWIdleTimeout *QWIdleTimeout::from(wlr_idle_timeout *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWIdleTimeout(handle, false);
}

QWIdleTimeout *QWIdleTimeout::create(QWIdle *idle, QWSeat *seat, uint32_t timeout)
{
    auto *handle = wlr_idle_timeout_create(idle->handle(), seat->handle(), timeout);
    if (!handle)
        return nullptr;
    return from(handle);
}

QW_END_NAMESPACE
