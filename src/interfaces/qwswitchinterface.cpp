// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwswitchinterface.h"

extern "C" {
#include <wlr/interfaces/wlr_switch.h>
#include <wayland-server-core.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_switch : public wlr_switch
{
    _wlr_switch(QWSwitchInterface *i)
        : interface(i)
    {}

    QWSwitchInterface *interface;
};

void QWSwitchInterface::notifyToggle() const {
    wl_signal_emit_mutable(&handle()->events.toggle, nullptr);
}

void QWSwitchInterface::init(FuncMagicKey funMagicKey, const char* name)
{
    Q_UNUSED(funMagicKey);
    auto impl = new wlr_switch_impl {
       .name = this->name()
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_switch));
    static_cast<_wlr_switch *>(m_handle)->interface = this;
    wlr_switch_init(handle(), impl, name);
}

QW_END_NAMESPACE
