// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_idle_inhibit_manager_v1;
struct wlr_idle_inhibitor_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWIdleInhibitorV1;
class QWIdleInhibitManagerV1Private;
class QW_EXPORT QWIdleInhibitManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWIdleInhibitManagerV1)
public:
    inline wlr_idle_inhibit_manager_v1 *handle() const {
        return QWObject::handle<wlr_idle_inhibit_manager_v1>();
    }

    static QWIdleInhibitManagerV1 *get(wlr_idle_inhibit_manager_v1 *handle);
    static QWIdleInhibitManagerV1 *from(wlr_idle_inhibit_manager_v1 *handle);
    static QWIdleInhibitManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWIdleInhibitManagerV1 *self);
    void newInhibitor(QWIdleInhibitorV1 *inhibitor);

private:
    QWIdleInhibitManagerV1(wlr_idle_inhibit_manager_v1 *handle, bool isOwner);
    ~QWIdleInhibitManagerV1() = default;
};

class QWIdleInhibitorV1Private;
class QW_EXPORT QWIdleInhibitorV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWIdleInhibitorV1)
public:
    inline wlr_idle_inhibitor_v1 *handle() const {
        return QWObject::handle<wlr_idle_inhibitor_v1>();
    }

    static QWIdleInhibitorV1 *get(wlr_idle_inhibitor_v1 *handle);
    static QWIdleInhibitorV1 *from(wlr_idle_inhibitor_v1 *handle);

Q_SIGNALS:
    void beforeDestroy(QWIdleInhibitorV1 *self);

private:
    QWIdleInhibitorV1(wlr_idle_inhibitor_v1 *handle, bool isOwner);
    ~QWIdleInhibitorV1() = default;
};

QW_END_NAMESPACE

