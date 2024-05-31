// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "qwkeyboard.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
}

QW_BEGIN_NAMESPACE

class QWInputMethodKeyboardGrabV2Private : public QWWrapObjectPrivate
{
public:
    QWInputMethodKeyboardGrabV2Private(wlr_input_method_keyboard_grab_v2 *handle, bool isOwner, QWInputMethodKeyboardGrabV2 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy,
                              toDestroyFunction(wlr_input_method_keyboard_grab_v2_destroy))
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWInputMethodKeyboardGrabV2)
};
QHash<void*, QWWrapObject*> QWInputMethodKeyboardGrabV2Private::map;

QWInputMethodKeyboardGrabV2::QWInputMethodKeyboardGrabV2(wlr_input_method_keyboard_grab_v2 *handle, bool isOwner)
    : QWWrapObject(*new QWInputMethodKeyboardGrabV2Private(handle, isOwner, this))
{

}

QWInputMethodKeyboardGrabV2 *QWInputMethodKeyboardGrabV2::get(wlr_input_method_keyboard_grab_v2 *handle)
{
    return static_cast<QWInputMethodKeyboardGrabV2*>(QWInputMethodKeyboardGrabV2Private::map.value(handle));
}

QWInputMethodKeyboardGrabV2 *QWInputMethodKeyboardGrabV2::from(wlr_input_method_keyboard_grab_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputMethodKeyboardGrabV2(handle, false);
}

void QWInputMethodKeyboardGrabV2::sendKey(uint32_t time, uint32_t key, uint32_t state)
{
    wlr_input_method_keyboard_grab_v2_send_key(handle(), time, key, state);
}

void QWInputMethodKeyboardGrabV2::sendModifiers(wlr_keyboard_modifiers *modifiers)
{
    wlr_input_method_keyboard_grab_v2_send_modifiers(handle(), modifiers);
}

void QWInputMethodKeyboardGrabV2::setKeyboard(QWKeyboard *keyboard)
{
    wlr_input_method_keyboard_grab_v2_set_keyboard(handle(), keyboard->handle());
}

QW_END_NAMESPACE
