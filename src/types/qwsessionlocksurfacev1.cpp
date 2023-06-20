// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>
#include <QSize>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockSurfaceV1Private : public QWObjectPrivate
{
public:
    QWSessionLockSurfaceV1Private(wlr_session_lock_surface_v1 *handle, bool isOwner, QWSessionLockSurfaceV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSessionLockSurfaceV1Private::on_destroy);
    }
    ~QWSessionLockSurfaceV1Private() {
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

    static QHash<void*, QWSessionLockSurfaceV1*> map;
    QW_DECLARE_PUBLIC(QWSessionLockSurfaceV1)
    QWSignalConnector sc;
};
QHash<void*, QWSessionLockSurfaceV1*> QWSessionLockSurfaceV1Private::map;

void QWSessionLockSurfaceV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWSessionLockSurfaceV1::QWSessionLockSurfaceV1(wlr_session_lock_surface_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSessionLockSurfaceV1Private(handle, isOwner, this))
{

}

QWSessionLockSurfaceV1 *QWSessionLockSurfaceV1::get(wlr_session_lock_surface_v1 *handle)
{
    return QWSessionLockSurfaceV1Private::map.value(handle);
}

QWSessionLockSurfaceV1 *QWSessionLockSurfaceV1::from(wlr_session_lock_surface_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSessionLockSurfaceV1(handle, false);
}

uint32_t QWSessionLockSurfaceV1::configure(const QSize &size)
{
    return wlr_session_lock_surface_v1_configure(handle(), size.width(), size.height());
}

QW_END_NAMESPACE
