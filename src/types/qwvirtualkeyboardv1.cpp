// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualkeyboardv1.h"
#include "util/qwsignalconnector.h"

#include <qwinputdevice.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
}

QW_BEGIN_NAMESPACE

QWVirtualKeyboardV1 *QWVirtualKeyboardV1::from(wlr_virtual_keyboard_v1 *handle)
{
    return reinterpret_cast<QWVirtualKeyboardV1*>(handle);
}

QWVirtualKeyboardV1 *QWVirtualKeyboardV1::fromInputDevice(QWInputDevice *inputDevice)
{
    return from(wlr_input_device_get_virtual_keyboard(inputDevice->handle()));
}

QW_END_NAMESPACE
