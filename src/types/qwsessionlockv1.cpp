// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockV1Private : public QWObjectPrivate
{
public:
    QWSessionLockV1Private(wlr_session_lock_v1 *handle, bool isOwner, QWSessionLockV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_surface, this, &QWSessionLockV1Private::on_new_surface);
        sc.connect(&handle->events.destroy, this, &QWSessionLockV1Private::on_destroy);
    }
    ~QWSessionLockV1Private() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_session_lock_v1_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(wlr_session_lock_surface_v1 *);

    static QHash<void*, QWSessionLockV1*> map;
    QW_DECLARE_PUBLIC(QWSessionLockV1)
    QWSignalConnector sc;
};
QHash<void*, QWSessionLockV1*> QWSessionLockV1Private::map;

void QWSessionLockV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWSessionLockV1Private::on_new_surface(wlr_session_lock_surface_v1 *surface)
{
    Q_EMIT q_func()->newSurface(QWSessionLockSurfaceV1::from(surface));
}

QWSessionLockV1::QWSessionLockV1(wlr_session_lock_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSessionLockV1Private(handle, isOwner, this))
{

}

QWSessionLockV1 *QWSessionLockV1::get(wlr_session_lock_v1 *handle)
{
    return QWSessionLockV1Private::map.value(handle);
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
