// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwswitch.h"

#include <qwinputdevice_p.h>
#include <qwbuffer.h>
#include <qwinputdevice.h>

#include <QHash>

extern "C" {
#include <wlr/types/wlr_switch.h>
}

QW_BEGIN_NAMESPACE

class QWSwitchPrivate : public QWInputDevicePrivate
{
public:
    QWSwitchPrivate(wlr_switch *handle, bool isOwner, QWSwitch *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.toggle, q_func(), &QWSwitch::toggle);
    }

    ~QWSwitchPrivate() override = default;

    QW_DECLARE_PUBLIC(QWSwitch)
};

QWSwitch *QWSwitch::get(wlr_switch *handle)
{
    return qobject_cast<QWSwitch*>(QWInputDevice::get(&handle->base));
}

QWSwitch *QWSwitch::from(wlr_switch *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSwitch(handle, false);
}

QWSwitch *QWSwitch::fromInputDevice(wlr_input_device *inputDevice)
{
    auto *handle = wlr_switch_from_input_device(inputDevice);
    return handle ? from(handle) : nullptr;
}

QWSwitch::QWSwitch(wlr_switch *handle, bool isOwner)
    : QWInputDevice(*new QWSwitchPrivate(handle, isOwner, this))
{
}

QW_END_NAMESPACE
