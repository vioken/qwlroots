// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "qwkeyboard.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
}

QW_BEGIN_NAMESPACE

class QWInputMethodKeyboardGrabV2Private : public QWObjectPrivate
{
public:
    QWInputMethodKeyboardGrabV2Private(wlr_input_method_keyboard_grab_v2 *handle, bool isOwner, QWInputMethodKeyboardGrabV2 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWInputMethodKeyboardGrabV2Private::on_destroy);
    }
    ~QWInputMethodKeyboardGrabV2Private() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_input_method_keyboard_grab_v2_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWInputMethodKeyboardGrabV2*> map;
    QW_DECLARE_PUBLIC(QWInputMethodKeyboardGrabV2)
    QWSignalConnector sc;
};
QHash<void*, QWInputMethodKeyboardGrabV2*> QWInputMethodKeyboardGrabV2Private::map;

void QWInputMethodKeyboardGrabV2Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWInputMethodKeyboardGrabV2::QWInputMethodKeyboardGrabV2(wlr_input_method_keyboard_grab_v2 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWInputMethodKeyboardGrabV2Private(handle, isOwner, this))
{

}

QWInputMethodKeyboardGrabV2 *QWInputMethodKeyboardGrabV2::get(wlr_input_method_keyboard_grab_v2 *handle)
{
    return QWInputMethodKeyboardGrabV2Private::map.value(handle);
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
