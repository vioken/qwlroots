// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrelativepointerv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <qwseat.h>
#include <QHash>
#include <QPointF>

extern "C" {
#include <wlr/types/wlr_relative_pointer_v1.h>
}

QW_BEGIN_NAMESPACE

class QWRelativeManagerV1Private : public QWObjectPrivate
{
public:
    QWRelativeManagerV1Private(wlr_relative_pointer_manager_v1 *handle, bool isOwner, QWRelativeManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWRelativeManagerV1Private::on_destroy);
    }
    ~QWRelativeManagerV1Private() {
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

    static QHash<void*, QWRelativeManagerV1*> map;
    QW_DECLARE_PUBLIC(QWRelativeManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWRelativeManagerV1*> QWRelativeManagerV1Private::map;

void QWRelativeManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWRelativeManagerV1::QWRelativeManagerV1(wlr_relative_pointer_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWRelativeManagerV1Private(handle, isOwner, this))
{

}

QWRelativeManagerV1 *QWRelativeManagerV1::get(wlr_relative_pointer_manager_v1 *handle)
{
    return QWRelativeManagerV1Private::map.value(handle);
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

