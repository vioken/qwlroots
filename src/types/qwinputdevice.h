// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_tablet_pad.h>
#include <wlr/types/wlr_tablet_tool.h>
#include <wlr/types/wlr_touch.h>
#include <wlr/types/wlr_switch.h>
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
        Q_ASSERT(T::from_input_device(handle));
        return new T(handle, false);
    }

    static qw_input_device *from(wlr_input_device *handle);
};

#define QW_INPUT_DEVICE(name) \
typedef qw_##name DeriveType; \
public: \
    QW_ALWAYS_INLINE wlr_##name *handle() const { \
        return from_input_device(qw_input_device::handle()); \
    } \
    QW_ALWAYS_INLINE operator wlr_##name* () const { \
        return handle(); \
    } \
    QW_FUNC_STATIC(name, from_input_device) \
protected: \
using qw_input_device::qw_input_device; \
private: \
friend class qw_input_device;

class qw_keyboard : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(keyboard)

    QW_SIGNAL(key, wlr_keyboard_key_event*)
    QW_SIGNAL(modifiers)
    QW_SIGNAL(keymap)
    QW_SIGNAL(repeat_info)

public:
    QW_FUNC_MEMBER(keyboard, get_modifiers)
    QW_FUNC_MEMBER(keyboard, set_keymap)
    QW_FUNC_MEMBER(keyboard, set_repeat_info)
};

class qw_pointer : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(pointer)

    QW_SIGNAL(motion, wlr_pointer_motion_event*)
    QW_SIGNAL(motion_absolute, wlr_pointer_motion_absolute_event*)
    QW_SIGNAL(button, wlr_pointer_button_event*)
    QW_SIGNAL(axis, wlr_pointer_axis_event*)
    QW_SIGNAL(frame)
    QW_SIGNAL(swipe_begin, wlr_pointer_swipe_begin_event*)
    QW_SIGNAL(swipe_update, wlr_pointer_swipe_update_event*)
    QW_SIGNAL(swipe_end, wlr_pointer_swipe_end_event*)
    QW_SIGNAL(pinch_begin, wlr_pointer_pinch_begin_event*)
    QW_SIGNAL(pinch_update, wlr_pointer_pinch_update_event*)
    QW_SIGNAL(pinch_end, wlr_pointer_pinch_end_event*)
    QW_SIGNAL(hold_begin, wlr_pointer_hold_begin_event*)
    QW_SIGNAL(hold_end, wlr_pointer_hold_end_event*)
};

class qw_tablet : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(tablet)

    QW_SIGNAL(axis, wlr_tablet_tool_axis_event*)
    QW_SIGNAL(proximity, wlr_tablet_tool_proximity_event*)
    QW_SIGNAL(tip, wlr_tablet_tool_tip_event*)
    QW_SIGNAL(button, wlr_tablet_tool_button_event*)
};

class qw_tablet_pad : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(tablet_pad)

    QW_SIGNAL(button, wlr_tablet_pad_button_event*)
    QW_SIGNAL(ring, wlr_tablet_pad_ring_event*)
    QW_SIGNAL(strip, wlr_tablet_pad_strip_event*)
    QW_SIGNAL(attach_tablet, wlr_tablet_tool*)
};

class qw_switch : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(switch)

    QW_SIGNAL(toggle, wlr_switch_toggle_event*)
};

class qw_touch : public qw_input_device
{
    Q_OBJECT
    QW_INPUT_DEVICE(touch)

    QW_SIGNAL(down, wlr_touch_down_event*)
    QW_SIGNAL(up, wlr_touch_up_event*)
    QW_SIGNAL(motion, wlr_touch_motion_event*)
    QW_SIGNAL(cancel, wlr_touch_cancel_event*)
    QW_SIGNAL(frame)
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
