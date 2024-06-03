// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwidle.h"
#include "qwseat.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_idle.h>
}

QW_BEGIN_NAMESPACE

/// QWIdle
class QWIdlePrivate : public QWWrapObjectPrivate
{
public:
    QWIdlePrivate(wlr_idle *handle, bool isOwner, QWIdle *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events->destroy)
    {
        sc.connect(&handle->events.activity_notify, this, &QWIdlePrivate::on_activity_notify);
    }

    void on_activity_notify(void *);

    QW_DECLARE_PUBLIC(QWIdle)
};

void QWIdlePrivate::on_activity_notify(void *)
{
    Q_EMIT q_func()->activityNotify();
}

QWIdle::QWIdle(wlr_idle *handle, bool isOwner)
    : QWWrapObject(*new QWIdlePrivate(handle, isOwner, this))
{

}

QWIdle *QWIdle::get(wlr_idle *handle)
{
    return static_cast<QWIdle*>(QWIdlePrivate::map.value(handle));
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
class QWIdleTimeoutPrivate : public QWWrapObjectPrivate
{
public:
    QWIdleTimeoutPrivate(wlr_idle_timeout *handle, bool isOwner, QWIdleTimeout *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_idle_timeout_destroy))
    {
        sc.connect(&handle->events.idle, this, &QWIdleTimeoutPrivate::on_idle);
        sc.connect(&handle->events.resume, this, &QWIdleTimeoutPrivate::on_resume);
    }

    void on_idle(void *);
    void on_resume(void *);

    QW_DECLARE_PUBLIC(QWIdleTimeout)
};

void QWIdleTimeoutPrivate::on_idle(void *)
{
    Q_EMIT q_func()->idle();
}

void QWIdleTimeoutPrivate::on_resume(void *)
{
    Q_EMIT q_func()->resume();
}

QWIdleTimeout::QWIdleTimeout(wlr_idle_timeout *handle, bool isOwner)
    : QWWrapObject(*new QWIdleTimeoutPrivate(handle, isOwner, this))
{

}

QWIdleTimeout *QWIdleTimeout::get(wlr_idle_timeout *handle)
{
    return static_cast<QWIdleTimeout*>(QWIdleTimeoutPrivate::map.value(handle));
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
