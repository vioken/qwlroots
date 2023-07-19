// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwidleinhibitv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_idle_inhibit_v1.h>
}

QW_BEGIN_NAMESPACE

/// QWIdleInhibitManagerV1

class QWIdleInhibitManagerV1Private : public QWObjectPrivate
{
public:
    QWIdleInhibitManagerV1Private(wlr_idle_inhibit_manager_v1 *handle, bool isOwner, QWIdleInhibitManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWIdleInhibitManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_inhibitor, this, &QWIdleInhibitManagerV1Private::on_new_inhibitor);
    }
    ~QWIdleInhibitManagerV1Private() {
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
    void on_new_inhibitor(void *);

    static QHash<void*, QWIdleInhibitManagerV1*> map;
    QW_DECLARE_PUBLIC(QWIdleInhibitManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWIdleInhibitManagerV1*> QWIdleInhibitManagerV1Private::map;

void QWIdleInhibitManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWIdleInhibitManagerV1Private::on_new_inhibitor(void *data)
{
    auto *inhibitor = QWIdleInhibitorV1::from(static_cast<wlr_idle_inhibitor_v1*>(data));
    Q_EMIT q_func()->newInhibitor(inhibitor);
}

QWIdleInhibitManagerV1::QWIdleInhibitManagerV1(wlr_idle_inhibit_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWIdleInhibitManagerV1Private(handle, isOwner, this))
{

}

QWIdleInhibitManagerV1 *QWIdleInhibitManagerV1::get(wlr_idle_inhibit_manager_v1 *handle)
{
    return QWIdleInhibitManagerV1Private::map.value(handle);
}

QWIdleInhibitManagerV1 *QWIdleInhibitManagerV1::from(wlr_idle_inhibit_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWIdleInhibitManagerV1(handle, false);
}

QWIdleInhibitManagerV1 *QWIdleInhibitManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_idle_inhibit_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWIdleInhibitManagerV1(handle, true);
}

/// QWIdleInhibitManagerV1

class QWIdleInhibitorV1Private : public QWObjectPrivate
{
public:
    QWIdleInhibitorV1Private(wlr_idle_inhibitor_v1 *handle, bool isOwner, QWIdleInhibitorV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWIdleInhibitorV1Private::on_destroy);
    }
    ~QWIdleInhibitorV1Private() {
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
    void on_new_inhibitor(void *);

    static QHash<void*, QWIdleInhibitorV1*> map;
    QW_DECLARE_PUBLIC(QWIdleInhibitorV1)
    QWSignalConnector sc;
};
QHash<void*, QWIdleInhibitorV1*> QWIdleInhibitorV1Private::map;

void QWIdleInhibitorV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWIdleInhibitorV1::QWIdleInhibitorV1(wlr_idle_inhibitor_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWIdleInhibitorV1Private(handle, isOwner, this))
{

}

QWIdleInhibitorV1 *QWIdleInhibitorV1::get(wlr_idle_inhibitor_v1 *handle)
{
    return QWIdleInhibitorV1Private::map.value(handle);
}

QWIdleInhibitorV1 *QWIdleInhibitorV1::from(wlr_idle_inhibitor_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWIdleInhibitorV1(handle, false);
}

QW_END_NAMESPACE
