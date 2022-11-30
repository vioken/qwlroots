// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

QW_BEGIN_NAMESPACE

class QWDataDeviceManagerPrivate : public QWObjectPrivate
{
public:
    QWDataDeviceManagerPrivate(wlr_data_device_manager *handle, QWDataDeviceManager *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&handle->events.destroy, this, &QWDataDeviceManagerPrivate::on_destroy);
    }
    ~QWDataDeviceManagerPrivate() {
        sc.invalidate();
    }

    void on_destroy(void *);

    QW_DECLARE_PUBLIC(QWDataDeviceManager)
    QWSignalConnector sc;
};

void QWDataDeviceManagerPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

QWDataDeviceManager::QWDataDeviceManager(wlr_data_device_manager *handle)
    : QObject(nullptr)
    , QWObject(*new QWDataDeviceManagerPrivate(handle, this))
{

}

QWDataDeviceManager *QWDataDeviceManager::create(QWDisplay *display)
{
    auto handle = wlr_data_device_manager_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWDataDeviceManager(handle);
}

QW_END_NAMESPACE
