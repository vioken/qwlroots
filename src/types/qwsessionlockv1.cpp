// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"

#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockV1Private : public QWWrapObjectPrivate
{
public:
    QWSessionLockV1Private(wlr_session_lock_v1 *handle, bool isOwner, QWSessionLockV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_session_lock_v1_destroy))
    {
        sc.connect(&handle->events.new_surface, this, &QWSessionLockV1Private::on_new_surface);
        sc.connect(&handle->events.unlock, this, &QWSessionLockV1Private::on_unlock);
    }

    void on_new_surface(wlr_session_lock_surface_v1 *);
    void on_unlock();

    QW_DECLARE_PUBLIC(QWSessionLockV1)
};


void QWSessionLockV1Private::on_new_surface(wlr_session_lock_surface_v1 *surface)
{
    Q_EMIT q_func()->newSurface(QWSessionLockSurfaceV1::from(surface));
}

void QWSessionLockV1Private::on_unlock()
{
    Q_EMIT q_func()->unlock();
}

QWSessionLockV1::QWSessionLockV1(wlr_session_lock_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWSessionLockV1Private(handle, isOwner, this))
{
}

QWSessionLockV1 *QWSessionLockV1::get(wlr_session_lock_v1 *handle)
{
    return static_cast<QWSessionLockV1*>(QWSessionLockV1Private::map.value(handle));
}

QWSessionLockV1 *QWSessionLockV1::from(wlr_session_lock_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSessionLockV1(handle, false);
}

void QWSessionLockV1::sendLocked()
{
    wlr_session_lock_v1_send_locked(handle());
}

QW_END_NAMESPACE
