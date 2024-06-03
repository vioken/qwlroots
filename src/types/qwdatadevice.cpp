// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

QW_BEGIN_NAMESPACE

class QWDataDeviceManagerPrivate : public QWWrapObjectPrivate
{
public:
    QWDataDeviceManagerPrivate(wlr_data_device_manager *handle, bool isOwner, QWDataDeviceManager *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWDataDeviceManager)
};

QWDataDeviceManager::QWDataDeviceManager(wlr_data_device_manager *handle, bool isOwner)
    : QWWrapObject(*new QWDataDeviceManagerPrivate(handle, isOwner, this))
{

}

QWDataDeviceManager *QWDataDeviceManager::get(wlr_data_device_manager *handle)
{
    return static_cast<QWDataDeviceManager*>(QWDataDeviceManagerPrivate::map.value(handle));
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
