// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputinhibitmanager.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>

#include <QHash>

extern "C" {
#include <wlr/types/wlr_input_inhibitor.h>
}

QW_BEGIN_NAMESPACE

class QWInputInhibitManagerPrivate : public QWObjectPrivate
{
public:
    QWInputInhibitManagerPrivate(wlr_input_inhibit_manager *handle, bool isOwner, QWInputInhibitManager *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWInputInhibitManagerPrivate::on_destroy);
        sc.connect(&handle->events.activate, this, &QWInputInhibitManagerPrivate::on_activate);
        sc.connect(&handle->events.deactivate, this, &QWInputInhibitManagerPrivate::on_deactivate);
    }
    ~QWInputInhibitManagerPrivate() {
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
    void on_activate(void *);
    void on_deactivate(void *);

    static QHash<void*, QWInputInhibitManager*> map;
    QW_DECLARE_PUBLIC(QWInputInhibitManager)
    QWSignalConnector sc;
};
QHash<void*, QWInputInhibitManager*> QWInputInhibitManagerPrivate::map;

void QWInputInhibitManagerPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWInputInhibitManagerPrivate::on_activate(void *)
{
    // self
    Q_EMIT q_func()->activate();
}

void QWInputInhibitManagerPrivate::on_deactivate(void *)
{
    // self
    Q_EMIT q_func()->deactivate();
}

QWInputInhibitManager::QWInputInhibitManager(wlr_input_inhibit_manager *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWInputInhibitManagerPrivate(handle, isOwner, this))
{

}

QWInputInhibitManager *QWInputInhibitManager::get(wlr_input_inhibit_manager *handle)
{
    return QWInputInhibitManagerPrivate::map.value(handle);
}

 QWInputInhibitManager *QWInputInhibitManager::from(wlr_input_inhibit_manager *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputInhibitManager(handle, false);
}

QWInputInhibitManager *QWInputInhibitManager::create(QWDisplay *display)
{
    auto *handle = wlr_input_inhibit_manager_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWInputInhibitManager(handle, true);
}

QW_END_NAMESPACE
