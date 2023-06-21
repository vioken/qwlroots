// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointerconstraintsv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <qwseat.h>
#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPointerConstraintsV1Private : public QWObjectPrivate
{
public:
    QWPointerConstraintsV1Private(wlr_pointer_constraints_v1 *handle, bool isOwner, QWPointerConstraintsV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_constraint, this, &QWPointerConstraintsV1Private::new_constraint);
    }
    ~QWPointerConstraintsV1Private() {
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

    void new_constraint(wlr_pointer_constraint_v1 *);

    static QHash<void*, QWPointerConstraintsV1*> map;
    QW_DECLARE_PUBLIC(QWPointerConstraintsV1)
    QWSignalConnector sc;
};
QHash<void*, QWPointerConstraintsV1*> QWPointerConstraintsV1Private::map;

void QWPointerConstraintsV1Private::new_constraint(wlr_pointer_constraint_v1 *constraint)
{
    Q_EMIT q_func()->newConstraint(QWPointerConstraintV1::from(constraint));
}

QWPointerConstraintsV1::QWPointerConstraintsV1(wlr_pointer_constraints_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWPointerConstraintsV1Private(handle, isOwner, this))
{

}

QWPointerConstraintsV1 *QWPointerConstraintsV1::get(wlr_pointer_constraints_v1 *handle)
{
    return QWPointerConstraintsV1Private::map.value(handle);
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
