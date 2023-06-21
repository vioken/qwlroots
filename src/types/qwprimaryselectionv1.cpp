// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwprimaryselectionv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_primary_selection_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPrimarySelectionV1DeviceManagerPrivate : public QWObjectPrivate
{
public:
    QWPrimarySelectionV1DeviceManagerPrivate(wlr_primary_selection_v1_device_manager *handle, bool isOwner, QWPrimarySelectionV1DeviceManager *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWPrimarySelectionV1DeviceManagerPrivate::on_destroy);
    }
    ~QWPrimarySelectionV1DeviceManagerPrivate() {
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

    static QHash<void*, QWPrimarySelectionV1DeviceManager*> map;
    QW_DECLARE_PUBLIC(QWPrimarySelectionV1DeviceManager)
    QWSignalConnector sc;
};
QHash<void*, QWPrimarySelectionV1DeviceManager*> QWPrimarySelectionV1DeviceManagerPrivate::map;

void QWPrimarySelectionV1DeviceManagerPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWPrimarySelectionV1DeviceManager::QWPrimarySelectionV1DeviceManager(wlr_primary_selection_v1_device_manager *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWPrimarySelectionV1DeviceManagerPrivate(handle, isOwner, this))
{

}

QWPrimarySelectionV1DeviceManager *QWPrimarySelectionV1DeviceManager::get(wlr_primary_selection_v1_device_manager *handle)
{
    return QWPrimarySelectionV1DeviceManagerPrivate::map.value(handle);
}

QWPrimarySelectionV1DeviceManager *QWPrimarySelectionV1DeviceManager::from(wlr_primary_selection_v1_device_manager *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPrimarySelectionV1DeviceManager(handle, false);
}

QWPrimarySelectionV1DeviceManager *QWPrimarySelectionV1DeviceManager::create(QWDisplay *display)
{
    auto *handle = wlr_primary_selection_v1_device_manager_create(display->handle());
    return handle ? new QWPrimarySelectionV1DeviceManager(handle, true) : nullptr;
}

QW_END_NAMESPACE

