// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

extern "C" {
#include <wlr/interfaces/wlr_tablet_pad.h>
#include <wayland-server-core.h>
}

QW_BEGIN_NAMESPACE

template<typename Derive>
class QW_CLASS_INTERFACE(tablet_pad)
{
    QW_INTERFACE_INIT(tablet_pad)

public:
    QW_INTERFACE_FUNC_STATIC(tablet_pad, init, void, wlr_tablet_pad *pad, const wlr_tablet_pad_impl *impl, const char *name)

    QW_INTERFACE(name)
};

QW_END_NAMESPACE
