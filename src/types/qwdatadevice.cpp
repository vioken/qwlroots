// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

QW_BEGIN_NAMESPACE

class QWDataDeviceManagerPrivate : public QWObjectPrivate
{
public:
    QWDataDeviceManagerPrivate(wlr_data_device_manager *handle, bool isOwner, QWDataDeviceManager *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWDataDeviceManagerPrivate::on_destroy);
    }
    ~QWDataDeviceManagerPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWDataDeviceManager(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWDataDeviceManager*> map;
    QW_DECLARE_PUBLIC(QWDataDeviceManager)
    QWSignalConnector sc;
};
QHash<void*, QWDataDeviceManager*> QWDataDeviceManagerPrivate::map;

void QWDataDeviceManagerPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWDataDeviceManager::QWDataDeviceManager(wlr_data_device_manager *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWDataDeviceManagerPrivate(handle, isOwner, this))
{

}

QWDataDeviceManager *QWDataDeviceManager::get(wlr_data_device_manager *handle)
{
    return QWDataDeviceManagerPrivate::map.value(handle);
}

QWDataDeviceManager *QWDataDeviceManager::from(wlr_data_device_manager *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDataDeviceManager(handle, false);
}

QWDataDeviceManager *QWDataDeviceManager::create(QWDisplay *display)
{
    auto handle = wlr_data_device_manager_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWDataDeviceManager(handle, true);
}

QW_END_NAMESPACE
