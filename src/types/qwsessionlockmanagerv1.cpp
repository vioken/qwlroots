// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockManagerV1Private : public QWObjectPrivate
{
public:
    QWSessionLockManagerV1Private(wlr_session_lock_manager_v1 *handle, bool isOwner, QWSessionLockManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_lock, this, &QWSessionLockManagerV1Private::on_new_lock);
        sc.connect(&handle->events.destroy, this, &QWSessionLockManagerV1Private::on_destroy);
    }
    ~QWSessionLockManagerV1Private() {
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
    void on_new_lock(wlr_session_lock_v1 *);

    static QHash<void*, QWSessionLockManagerV1*> map;
    QW_DECLARE_PUBLIC(QWSessionLockManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWSessionLockManagerV1*> QWSessionLockManagerV1Private::map;

void QWSessionLockManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWSessionLockManagerV1Private::on_new_lock(wlr_session_lock_v1 *lock)
{
    Q_EMIT q_func()->newLock(QWSessionLockV1::from(lock));
}

QWSessionLockManagerV1::QWSessionLockManagerV1(wlr_session_lock_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSessionLockManagerV1Private(handle, isOwner, this))
{

}

QWSessionLockManagerV1 *QWSessionLockManagerV1::get(wlr_session_lock_manager_v1 *handle)
{
    return QWSessionLockManagerV1Private::map.value(handle);
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
