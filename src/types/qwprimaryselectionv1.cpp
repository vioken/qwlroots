// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwprimaryselectionv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_primary_selection_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPrimarySelectionV1DeviceManagerPrivate : public QWWrapObjectPrivate
{
public:
    QWPrimarySelectionV1DeviceManagerPrivate(wlr_primary_selection_v1_device_manager *handle, bool isOwner, QWPrimarySelectionV1DeviceManager *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWPrimarySelectionV1DeviceManager)
};

QWPrimarySelectionV1DeviceManager::QWPrimarySelectionV1DeviceManager(wlr_primary_selection_v1_device_manager *handle, bool isOwner)
    : QWWrapObject(*new QWPrimarySelectionV1DeviceManagerPrivate(handle, isOwner, this))
{

}

QWPrimarySelectionV1DeviceManager *QWPrimarySelectionV1DeviceManager::get(wlr_primary_selection_v1_device_manager *handle)
{
    return static_cast<QWPrimarySelectionV1DeviceManager*>(QWPrimarySelectionV1DeviceManagerPrivate::map.value(handle));
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

