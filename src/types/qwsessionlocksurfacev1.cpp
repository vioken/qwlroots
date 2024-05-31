// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsessionlockv1.h"
#include "private/qwglobal_p.h"

#include <QHash>
#include <QSize>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSessionLockSurfaceV1Private : public QWWrapObjectPrivate
{
public:
    QWSessionLockSurfaceV1Private(wlr_session_lock_surface_v1 *handle, bool isOwner, QWSessionLockSurfaceV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWSessionLockSurfaceV1)
};
QHash<void*, QWWrapObject*> QWSessionLockSurfaceV1Private::map;

QWSessionLockSurfaceV1::QWSessionLockSurfaceV1(wlr_session_lock_surface_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWSessionLockSurfaceV1Private(handle, isOwner, this))
{

}

QWSessionLockSurfaceV1 *QWSessionLockSurfaceV1::get(wlr_session_lock_surface_v1 *handle)
{
    return static_cast<QWSessionLockSurfaceV1*>(QWSessionLockSurfaceV1Private::map.value(handle));
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
