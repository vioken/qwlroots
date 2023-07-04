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
    : QWObjectPrivate(handle, isOwner, qq)
{
    Q_ASSERT(!map.contains(handle));
    map.insert(handle, qq);
    sc.connect(&handle->events.destroy, this, &QWInputDevicePrivate::on_destroy);
}

QWInputDevicePrivate::~QWInputDevicePrivate()
{
    if (!m_handle)
        return;
    destroy();
}

void QWInputDevicePrivate::destroy() {
    Q_ASSERT(m_handle);
    Q_ASSERT(map.contains(m_handle));
    Q_EMIT q_func()->beforeDestroy(q_func());
    map.remove(m_handle);
    sc.invalidate();
}

void QWInputDevicePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QHash<void*, QWInputDevice*> QWInputDevicePrivate::map;

QWInputDevice::QWInputDevice(QWObjectPrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{

}

QWInputDevice::QWInputDevice(wlr_input_device *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWInputDevicePrivate(handle, isOwner, this))
{

}

QWInputDevice *QWInputDevice::get(wlr_input_device *handle)
{
    return QWInputDevicePrivate::map.value(handle);
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
    case WLR_INPUT_DEVICE_TABLET_TOOL:
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
