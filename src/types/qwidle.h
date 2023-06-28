// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_idle;
struct wlr_idle_timeout;

QW_BEGIN_NAMESPACE
class QWSeat;
class QWDisplay;
class QWIdlePrivate;
class QW_EXPORT QWIdle : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWIdle)
public:
    inline wlr_idle *handle() const {
        return QWObject::handle<wlr_idle>();
    }

    static QWIdle *get(wlr_idle *handle);
    static QWIdle *from(wlr_idle *handle);
    static QWIdle *create(QWDisplay *display);

    void notifyActivity(QWSeat *seat);
    void setEnabled(QWSeat *seat, bool enabled);

Q_SIGNALS:
    void beforeDestroy(QWIdle *self);
    void activityNotify();

private:
    QWIdle(wlr_idle *handle, bool isOwner);
    ~QWIdle() = default;
};

class QWIdleTimeoutPrivate;
class QW_EXPORT QWIdleTimeout : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWIdleTimeout)
public:
    ~QWIdleTimeout() = default;

    inline wlr_idle_timeout *handle() const {
        return QWObject::handle<wlr_idle_timeout>();
    }

    static QWIdleTimeout *get(wlr_idle_timeout *handle);
    static QWIdleTimeout *from(wlr_idle_timeout *handle);
    static QWIdleTimeout *create(QWIdle *idle, QWSeat *seat, uint32_t timeout);

Q_SIGNALS:
    void beforeDestroy(QWIdleTimeout *self);
    void idle();
    void resume();

private:
    QWIdleTimeout(wlr_idle_timeout *handle, bool isOwner);
};

QW_END_NAMESPACE
