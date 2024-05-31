// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualkeyboardv1.h"
#include "qwkeyboard_p.h"
#include "qwinputdevice.h"
#include "private/qwglobal_p.h"

extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
}

QW_BEGIN_NAMESPACE

class QWVirtualKeyboardV1Private : public QWKeyboardPrivate
{
public:
    QWVirtualKeyboardV1Private(wlr_virtual_keyboard_v1 *handle, bool isOwner, QWVirtualKeyboardV1 *qq)
        : QWKeyboardPrivate(&handle->keyboard, isOwner, qq)
    {

    }
    ~QWVirtualKeyboardV1Private() override = default;

    QW_DECLARE_PUBLIC(QWVirtualKeyboardV1)
};

QWVirtualKeyboardV1::QWVirtualKeyboardV1(wlr_virtual_keyboard_v1 *handle, bool isOwner)
    : QWKeyboard(*new QWVirtualKeyboardV1Private(handle, isOwner, this))
{

}

QWVirtualKeyboardV1 *QWVirtualKeyboardV1::get(wlr_virtual_keyboard_v1 *handle)
{
    return qobject_cast<QWVirtualKeyboardV1*>(QWKeyboard::get(&handle->keyboard));
}

QWVirtualKeyboardV1 *QWVirtualKeyboardV1::from(wlr_virtual_keyboard_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWVirtualKeyboardV1(handle, false);
}

QWVirtualKeyboardV1 *QWVirtualKeyboardV1::fromInputDevice(QWInputDevice *inputDevice)
{
    return from(wlr_input_device_get_virtual_keyboard(inputDevice->handle()));
}

QW_END_NAMESPACE
