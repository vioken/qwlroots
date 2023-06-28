// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_pointer_constraint_v1;
struct wlr_pointer_constraints_v1;

QW_BEGIN_NAMESPACE

class QWPointerConstraintV1Private;
class QW_EXPORT QWPointerConstraintV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPointerConstraintV1)

    friend class QWPointerConstraintsV1;
public:
    inline wlr_pointer_constraint_v1 *handle() const {
        return QWObject::handle<wlr_pointer_constraint_v1>();
    }

    static QWPointerConstraintV1 *get(wlr_pointer_constraint_v1 *handle);
    static QWPointerConstraintV1 *from(wlr_pointer_constraint_v1 *handle);

    void sendActivated();
    void sendDeactivated();

Q_SIGNALS:
    void beforeDestroy(QWPointerConstraintV1 *self);
    void setRegion();

private:
    QWPointerConstraintV1(wlr_pointer_constraint_v1 *handle, bool isOwner);
    ~QWPointerConstraintV1() = default;
};

class QWSeat;
class QWDisplay;
class QWSurface;
class QWPointerConstraintsV1Private;
class QW_EXPORT QWPointerConstraintsV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPointerConstraintsV1)
public:
    inline wlr_pointer_constraints_v1 *handle() const {
        return QWObject::handle<wlr_pointer_constraints_v1>();
    }

    static QWPointerConstraintsV1 *get(wlr_pointer_constraints_v1 *handle);
    static QWPointerConstraintsV1 *from(wlr_pointer_constraints_v1 *handle);
    static QWPointerConstraintsV1 *create(QWDisplay *display);

    QWPointerConstraintV1* constraintForSurface(QWSurface *surface, QWSeat *seat);

Q_SIGNALS:
    void beforeDestroy(QWPointerConstraintsV1 *self);
    void newConstraint(QWPointerConstraintV1 *);

private:
    QWPointerConstraintsV1(wlr_pointer_constraints_v1 *handle, bool isOwner);
    ~QWPointerConstraintsV1() = default;
};

QW_END_NAMESPACE

