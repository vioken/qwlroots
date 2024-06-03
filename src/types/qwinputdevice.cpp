// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputdevice.h"
#include "qwinputdevice_p.h"
#include "qwkeyboard.h"
#include "qwpointer.h"
#include "qwtablet.h"
#include "qwtabletpad.h"
#include "qwswitch.h"
#include "qwtouch.h"

extern "C" {
#include <wlr/types/wlr_input_device.h>
}

QW_BEGIN_NAMESPACE

QWInputDevicePrivate::QWInputDevicePrivate(wlr_input_device *handle, bool isOwner, QWInputDevice *qq)
    : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
{

}


QWInputDevice::QWInputDevice(QWInputDevicePrivate &dd)
    : QWWrapObject(dd)
{

}

QWInputDevice::QWInputDevice(wlr_input_device *handle, bool isOwner)
    : QWWrapObject(*new QWInputDevicePrivate(handle, isOwner, this))
{

}

QWInputDevice *QWInputDevice::get(wlr_input_device *handle)
{
    return static_cast<QWInputDevice*>(QWInputDevicePrivate::map.value(handle));
}

QWInputDevice *QWInputDevice::from(wlr_input_device *handle)
{
    if (auto o = get(handle))
        return o;

    switch (handle->type) {
    case WLR_INPUT_DEVICE_KEYBOARD:
        return QWKeyboard::fromInputDevice(handle);
    case WLR_INPUT_DEVICE_POINTER:
        return QWPointer::fromInputDevice(handle);
#if WLR_VERSION_MINOR > 17
    case WLR_INPUT_DEVICE_TABLET:
#else
    case WLR_INPUT_DEVICE_TABLET_TOOL:
#endif
        return QWTablet::fromInputDevice(handle);
    case WLR_INPUT_DEVICE_TABLET_PAD:
        return QWTabletPad::fromInputDevice(handle);
    case WLR_INPUT_DEVICE_SWITCH:
        return QWSwitch::fromInputDevice(handle);
    case WLR_INPUT_DEVICE_TOUCH:
        return QWTouch::fromInputDevice(handle);
    default:
        // Here is not reachable
        qCritical("Unknow input device type!");
        return nullptr;
    }
}

QW_END_NAMESPACE
