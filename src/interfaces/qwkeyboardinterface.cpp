// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardinterface.h"

extern "C" {
#include <wlr/interfaces/wlr_keyboard.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_keyboard : public wlr_keyboard
{
    _wlr_keyboard(QWKeyboardInterface *i)
        : interface(i) {}

    QWKeyboardInterface *interface;
};

inline static QWKeyboardInterface *interface(wlr_keyboard *handle) {
    auto _handle = static_cast<_wlr_keyboard*>(handle);
    Q_ASSERT(_handle->interface && _handle->impl == handle->impl);
    return _handle->interface;
}

namespace impl {

static void led_update(wlr_keyboard *handle, uint32_t leds) {
    return interface(handle)->ledUpdate(leds);
}

} // end namespace impl

QWKeyboardInterface::~QWKeyboardInterface()
{
    wlr_keyboard_finish(handle());
    free(handle());
    delete impl();
}

void QWKeyboardInterface::ledUpdate(uint32_t leds) const
{
    return;
}

QWKeyboardInterface *QWKeyboardInterface::get(wlr_keyboard *handle)
{
    return interface(handle);
}

void QWKeyboardInterface::init(FuncMagicKey funMagicKey)
{
    auto impl = new wlr_keyboard_impl {
        .name = name(),
        QW_INIT_INTERFACE_FUNC(funMagicKey, led_update, &QWKeyboardInterface::ledUpdate),
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_keyboard));
    static_cast<_wlr_keyboard *>(m_handle)->interface = this;
    wlr_keyboard_init(handle(), impl, name());
}

QW_END_NAMESPACE
