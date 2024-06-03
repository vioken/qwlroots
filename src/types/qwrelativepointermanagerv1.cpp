// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrelativepointerv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <qwseat.h>
#include <QHash>
#include <QPointF>

extern "C" {
#include <wlr/types/wlr_relative_pointer_v1.h>
}

QW_BEGIN_NAMESPACE

class QWRelativeManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWRelativeManagerV1Private(wlr_relative_pointer_manager_v1 *handle, bool isOwner, QWRelativeManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWRelativeManagerV1)
};

QWRelativeManagerV1::QWRelativeManagerV1(wlr_relative_pointer_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWRelativeManagerV1Private(handle, isOwner, this))
{

}

QWRelativeManagerV1 *QWRelativeManagerV1::get(wlr_relative_pointer_manager_v1 *handle)
{
    return static_cast<QWRelativeManagerV1*>(QWRelativeManagerV1Private::map.value(handle));
}

QWRelativeManagerV1 *QWRelativeManagerV1::from(wlr_relative_pointer_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWRelativeManagerV1(handle, false);
}

QWRelativeManagerV1 *QWRelativeManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_relative_pointer_manager_v1_create(display->handle());
    return handle ? new QWRelativeManagerV1(handle, true) : nullptr;
}

void QWRelativeManagerV1::sendRelativeMotion(QWSeat *seat, uint64_t time_usec, const QPointF &pos, const QPointF & unaccelPos)
{
    return wlr_relative_pointer_manager_v1_send_relative_motion(handle(), seat->handle(), time_usec, pos.x(), pos.y(), unaccelPos.x(), unaccelPos.y());
}

QW_END_NAMESPACE

