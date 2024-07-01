// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_input_device.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(input_device)
{
    QW_OBJECT
    Q_OBJECT

public:
    template<typename T>
    QW_ALWAYS_INLINE static std::enable_if<std::is_base_of<qw_input_device, T>::value, qw_input_device*>::type
    from(wlr_input_device *handle) {
        return new T(T::from_input_device(handle), false);
    }

    static qw_input_device *from(wlr_input_device *handle);
};

class qw_keyboard : public qw_input_device
{
    Q_OBJECT

    typedef qw_input_device DeriveType;

public:
    QW_SIGNAL(key, wlr_keyboard_key_event*)
    QW_SIGNAL(modifiers)
    QW_SIGNAL(keymap)
    QW_SIGNAL(repeat_info)

public:
    QW_FUNC_STATIC(keyboard, from_input_device)

    QW_FUNC_MEMBER(keyboard, get_modifiers)
    QW_FUNC_MEMBER(keyboard, set_keymap)
    QW_FUNC_MEMBER(keyboard, set_repeat_info)

protected:
    using qw_input_device::qw_input_device;
    friend class qw_input_device;
};

qw_input_device *qw_input_device::from(wlr_input_device *handle) {
    if (auto o = get(handle))
        return o;

    switch (handle->type) {
    case WLR_INPUT_DEVICE_KEYBOARD:
        return from<qw_keyboard>(handle);
    case WLR_INPUT_DEVICE_POINTER:
        return from<qw_pointer>(handle);
#if WLR_ERSION_MINOR > 17
    case WLR_INPUT_DEVICE_TABLET:
#else
    case WLR_INPUT_DEVICE_TABLET_TOOL:
#endif
        return from<qw_tablet>(handle);
    case WLR_INPUT_DEVICE_TABLET_PAD:
        return from<qw_tablet_pad>(handle);
    case WLR_INPUT_DEVICE_SWITCH:
        return from<qw_switch>(handle);
    case WLR_INPUT_DEVICE_TOUCH:
        return from<qw_touch>(handle);
    default:
        // Here is not reachable
        qCritical("Unknow input device type!");
        return nullptr;
    }
}

QW_END_NAMESPACE
