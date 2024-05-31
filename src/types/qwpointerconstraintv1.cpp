// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointerconstraintsv1.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPointerConstraintV1Private : public QWWrapObjectPrivate
{
public:
    QWPointerConstraintV1Private(wlr_pointer_constraint_v1 *handle, bool isOwner, QWPointerConstraintV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.set_region, this, &QWPointerConstraintV1Private::on_set_region);
    }

    void on_set_region(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWPointerConstraintV1)
};
QHash<void*, QWWrapObject*> QWPointerConstraintV1Private::map;

void QWPointerConstraintV1Private::on_set_region(void *)
{
    Q_EMIT q_func()->setRegion();
}

QWPointerConstraintV1::QWPointerConstraintV1(wlr_pointer_constraint_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWPointerConstraintV1Private(handle, isOwner, this))
{

}

QWPointerConstraintV1 *QWPointerConstraintV1::get(wlr_pointer_constraint_v1 *handle)
{
    return static_cast<QWPointerConstraintV1*>(QWPointerConstraintV1Private::map.value(handle));
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

