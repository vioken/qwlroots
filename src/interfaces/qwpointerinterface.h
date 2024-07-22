// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

extern "C" {
#include <wlr/interfaces/wlr_pointer.h>
#include <wayland-server-core.h>
}

QW_BEGIN_NAMESPACE

template<typename Derive>
class QW_CLASS_INTERFACE(pointer)
{
    QW_INTERFACE_INIT(pointer)

public:
    QW_INTERFACE_FUNC_STATIC(pointer, init, void, wlr_pointer *pointer, const wlr_pointer_impl *impl, const char *name)

    QW_INTERFACE(name)
};

QW_END_NAMESPACE
