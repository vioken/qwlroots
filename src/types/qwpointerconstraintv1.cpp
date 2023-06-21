// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointerconstraintsv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPointerConstraintV1Private : public QWObjectPrivate
{
public:
    QWPointerConstraintV1Private(wlr_pointer_constraint_v1 *handle, bool isOwner, QWPointerConstraintV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWPointerConstraintV1Private::on_destroy);
        sc.connect(&handle->events.set_region, this, &QWPointerConstraintV1Private::on_set_region);
    }
    ~QWPointerConstraintV1Private() {
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
    void on_set_region(void *);

    static QHash<void*, QWPointerConstraintV1*> map;
    QW_DECLARE_PUBLIC(QWPointerConstraintV1)
    QWSignalConnector sc;
};
QHash<void*, QWPointerConstraintV1*> QWPointerConstraintV1Private::map;

void QWPointerConstraintV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWPointerConstraintV1Private::on_set_region(void *)
{
    Q_EMIT q_func()->setRegion();
}

QWPointerConstraintV1::QWPointerConstraintV1(wlr_pointer_constraint_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWPointerConstraintV1Private(handle, isOwner, this))
{

}

QWPointerConstraintV1 *QWPointerConstraintV1::get(wlr_pointer_constraint_v1 *handle)
{
    return QWPointerConstraintV1Private::map.value(handle);
}

QWPointerConstraintV1 *QWPointerConstraintV1::from(wlr_pointer_constraint_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPointerConstraintV1(handle, false);
}

void QWPointerConstraintV1::sendActivated()
{
    wlr_pointer_constraint_v1_send_activated(handle());
}

void QWPointerConstraintV1::sendDeactivated()
{
    wlr_pointer_constraint_v1_send_deactivated(handle());
}

QW_END_NAMESPACE

