// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletpadinterface.h"

extern "C" {
#include <wlr/interfaces/wlr_tablet_pad.h>
#include <wayland-server-core.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_tablet_pad : public wlr_tablet_pad
{
    _wlr_tablet_pad(QWTabletPadInterface *i)
        : interface(i)
    {}

    QWTabletPadInterface *interface;
};

void QWTabletPadInterface::notifyButton()
{
    wl_signal_emit_mutable(&handle()->events.button, nullptr);
}

void QWTabletPadInterface::notifyRing()
{
    wl_signal_emit_mutable(&handle()->events.ring, nullptr);
}

void QWTabletPadInterface::notifyStrip()
{
    wl_signal_emit_mutable(&handle()->events.strip, nullptr);
}

void QWTabletPadInterface::notifyAttachTablet(wlr_tablet_tool *table)
{
    wl_signal_emit_mutable(&handle()->events.attach_tablet, table);
}

void QWTabletPadInterface::init(FuncMagicKey funMagicKey, const char* name)
{
    Q_UNUSED(funMagicKey);
    auto impl = new wlr_tablet_pad_impl {
       .name = this->name()
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_tablet_pad));
    static_cast<_wlr_tablet_pad *>(m_handle)->interface = this;
    wlr_tablet_pad_init(handle(), impl, name);
}

QW_END_NAMESPACE
