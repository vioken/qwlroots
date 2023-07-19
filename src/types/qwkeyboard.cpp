// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboard.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_keyboard.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardPrivate : public QWInputDevicePrivate
{
public:
    QWKeyboardPrivate(wlr_keyboard *handle, bool isOwner, QWKeyboard *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.key, this, &QWKeyboardPrivate::on_key);
        sc.connect(&handle->events.modifiers, this, &QWKeyboardPrivate::on_modifiers);
        sc.connect(&handle->events.keymap, this, &QWKeyboardPrivate::on_keymap);
        sc.connect(&handle->events.repeat_info, this, &QWKeyboardPrivate::on_repeat_info);
    }
    ~QWKeyboardPrivate() override = default;

    void on_key(void *);
    void on_modifiers(void *);
    void on_keymap(void *);
    void on_repeat_info(void *);

    QW_DECLARE_PUBLIC(QWKeyboard)
};

void QWKeyboardPrivate::on_key(void *data)
{
    Q_EMIT q_func()->key(reinterpret_cast<wlr_keyboard_key_event *>(data));
}

void QWKeyboardPrivate::on_modifiers(void *)
{
    Q_EMIT q_func()->modifiers();
}

void QWKeyboardPrivate::on_keymap(void *)
{
    Q_EMIT q_func()->keymapChanged();
}

void QWKeyboardPrivate::on_repeat_info(void *)
{
    Q_EMIT q_func()->repeatInfoChanged();
}

QWKeyboard::QWKeyboard(wlr_keyboard *handle, bool isOwner)
    : QWInputDevice(*new QWKeyboardPrivate(handle, isOwner, this))
{

}

QWKeyboard *QWKeyboard::get(wlr_keyboard *handle)
{
    return qobject_cast<QWKeyboard*>(QWInputDevice::get(&handle->base));
}

QWKeyboard *QWKeyboard::from(wlr_keyboard *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWKeyboard(handle, false);
}

QWKeyboard *QWKeyboard::fromInputDevice(wlr_input_device *input_device)
{
    wlr_keyboard *handle = wlr_keyboard_from_input_device(input_device);
    return from(handle);
}

uint32_t QWKeyboard::getModifiers() const
{
    return wlr_keyboard_get_modifiers(handle());
}

void QWKeyboard::setKeymap(xkb_keymap *keymap)
{
    wlr_keyboard_set_keymap(handle(), keymap);
}

void QWKeyboard::setRepeatInfo(int32_t rate, int32_t delay)
{
    wlr_keyboard_set_repeat_info(handle(), rate, delay);
}

QW_END_NAMESPACE
