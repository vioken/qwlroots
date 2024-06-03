// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWSessionLockManagerV1Private(wlr_session_lock_manager_v1 *handle, bool isOwner, QWSessionLockManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_lock, this, &QWSessionLockManagerV1Private::on_new_lock);
    }

    void on_new_lock(wlr_session_lock_v1 *);

    QW_DECLARE_PUBLIC(QWSessionLockManagerV1)
};

void QWSessionLockManagerV1Private::on_new_lock(wlr_session_lock_v1 *lock)
{
    Q_EMIT q_func()->newLock(QWSessionLockV1::from(lock));
}

QWSessionLockManagerV1::QWSessionLockManagerV1(wlr_session_lock_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWSessionLockManagerV1Private(handle, isOwner, this))
{

}

QWSessionLockManagerV1 *QWSessionLockManagerV1::get(wlr_session_lock_manager_v1 *handle)
{
    return static_cast<QWSessionLockManagerV1*>(QWSessionLockManagerV1Private::map.value(handle));
}

QWSessionLockManagerV1 *QWSessionLockManagerV1::from(wlr_session_lock_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSessionLockManagerV1(handle, false);
}

QWSessionLockManagerV1 *QWSessionLockManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_session_lock_manager_v1_create(display->handle());
    return handle ? new QWSessionLockManagerV1(handle, true) : nullptr;
}

QW_END_NAMESPACE
