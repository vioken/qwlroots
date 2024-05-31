// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointerconstraintsv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <qwseat.h>
#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPointerConstraintsV1Private : public QWWrapObjectPrivate
{
public:
    QWPointerConstraintsV1Private(wlr_pointer_constraints_v1 *handle, bool isOwner, QWPointerConstraintsV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map)
    {
        sc.connect(&handle->events.new_constraint, this, &QWPointerConstraintsV1Private::new_constraint);
    }

    void new_constraint(wlr_pointer_constraint_v1 *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWPointerConstraintsV1)
};
QHash<void*, QWWrapObject*> QWPointerConstraintsV1Private::map;

void QWPointerConstraintsV1Private::new_constraint(wlr_pointer_constraint_v1 *constraint)
{
    Q_EMIT q_func()->newConstraint(QWPointerConstraintV1::from(constraint));
}

QWPointerConstraintsV1::QWPointerConstraintsV1(wlr_pointer_constraints_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWPointerConstraintsV1Private(handle, isOwner, this))
{

}

QWPointerConstraintsV1 *QWPointerConstraintsV1::get(wlr_pointer_constraints_v1 *handle)
{
    return static_cast<QWPointerConstraintsV1*>(QWPointerConstraintsV1Private::map.value(handle));
}

QWPointerConstraintsV1 *QWPointerConstraintsV1::from(wlr_pointer_constraints_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPointerConstraintsV1(handle, false);
}

QWPointerConstraintsV1 *QWPointerConstraintsV1::create(QWDisplay *display)
{
    auto *handle = wlr_pointer_constraints_v1_create(display->handle());
    return handle ? new QWPointerConstraintsV1(handle, true) : nullptr;
}

QWPointerConstraintV1* QWPointerConstraintsV1::constraintForSurface(QWSurface *surface, QWSeat *seat)
{
    auto *handle = wlr_pointer_constraints_v1_constraint_for_surface(this->handle(), surface->handle(), seat->handle());
    return handle ? new QWPointerConstraintV1(handle, true) : nullptr;
}

QW_END_NAMESPACE
