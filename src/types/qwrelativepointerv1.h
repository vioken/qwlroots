// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_resource;
struct wlr_relative_pointer_v1;
struct wlr_relative_pointer_manager_v1;

QW_BEGIN_NAMESPACE
class QWRelativeV1Private;
class QW_EXPORT QWRelativeV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWRelativeV1)
public:
    inline wlr_relative_pointer_v1 *handle() const {
        return QWObject::handle<wlr_relative_pointer_v1>();
    }

    static QWRelativeV1 *get(wlr_relative_pointer_v1 *handle);
    static QWRelativeV1 *from(wlr_relative_pointer_v1 *handle);
    static QWRelativeV1 *fromResource(wl_resource *resource);

Q_SIGNALS:
    void beforeDestroy(QWRelativeV1 *self);

private:
    QWRelativeV1(wlr_relative_pointer_v1 *handle, bool isOwner);
    ~QWRelativeV1() = default;
};

class QWSeat;
class QWDisplay;
class QWRelativeManagerV1Private;
class QW_EXPORT QWRelativeManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWRelativeManagerV1)
public:
    inline wlr_relative_pointer_manager_v1 *handle() const {
        return QWObject::handle<wlr_relative_pointer_manager_v1>();
    }

    static QWRelativeManagerV1 *get(wlr_relative_pointer_manager_v1 *handle);
    static QWRelativeManagerV1 *from(wlr_relative_pointer_manager_v1 *handle);
    static QWRelativeManagerV1 *create(QWDisplay *display);

    void sendRelativeMotion(QWSeat *seat, uint64_t time_usec, const QPointF &pos, const QPointF & unaccelPos);

Q_SIGNALS:
    void beforeDestroy(QWRelativeManagerV1 *self);

private:
    QWRelativeManagerV1(wlr_relative_pointer_manager_v1 *handle, bool isOwner);
    ~QWRelativeManagerV1() = default;
};

QW_END_NAMESPACE

