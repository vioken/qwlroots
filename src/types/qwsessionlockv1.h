// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_session_lock_v1;
struct wlr_session_lock_surface_v1;
struct wlr_session_lock_manager_v1;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWSessionLockSurfaceV1Private;
class QW_EXPORT QWSessionLockSurfaceV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSessionLockSurfaceV1)
public:
    inline wlr_session_lock_surface_v1 *handle() const {
        return QWObject::handle<wlr_session_lock_surface_v1>();
    }

    static QWSessionLockSurfaceV1 *get(wlr_session_lock_surface_v1 *handle);
    static QWSessionLockSurfaceV1 *from(wlr_session_lock_surface_v1 *handle);
    static QWSessionLockSurfaceV1 *tryCreate(QWSurface *surface);

    uint32_t configure(const QSize &size);

Q_SIGNALS:
    void beforeDestroy(QWSessionLockSurfaceV1 *self);

private:
    QWSessionLockSurfaceV1(wlr_session_lock_surface_v1 *handle, bool isOwner);
    ~QWSessionLockSurfaceV1() = default;
};

class QWSessionLockV1Private;
class QW_EXPORT QWSessionLockV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSessionLockV1)
public:
    ~QWSessionLockV1() = default;

    inline wlr_session_lock_v1 *handle() const {
        return QWObject::handle<wlr_session_lock_v1>();
    }

    static QWSessionLockV1 *get(wlr_session_lock_v1 *handle);
    static QWSessionLockV1 *from(wlr_session_lock_v1 *handle);

    void sendLocked();

Q_SIGNALS:
    void beforeDestroy(QWSessionLockV1 *self);
    void newSurface(QWSessionLockSurfaceV1 *);
    void unlock();

private:
    QWSessionLockV1(wlr_session_lock_v1 *handle, bool isOwner);
};

class QWDisplay;
class QWSessionLockManagerV1Private;
class QW_EXPORT QWSessionLockManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSessionLockManagerV1)
public:
    inline wlr_session_lock_manager_v1 *handle() const {
        return QWObject::handle<wlr_session_lock_manager_v1>();
    }

    static QWSessionLockManagerV1 *get(wlr_session_lock_manager_v1 *handle);
    static QWSessionLockManagerV1 *from(wlr_session_lock_manager_v1 *handle);
    static QWSessionLockManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWSessionLockManagerV1 *self);
    void newLock(QWSessionLockV1 *);

private:
    QWSessionLockManagerV1(wlr_session_lock_manager_v1 *handle, bool isOwner);
    ~QWSessionLockManagerV1() = default;
};

QW_END_NAMESPACE

