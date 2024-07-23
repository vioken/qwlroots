// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

QW_BEGIN_NAMESPACE

extern "C" {
#include <wlr/interfaces/wlr_keyboard.h>
}

template<typename Derive>
class QW_CLASS_INTERFACE(keyboard)
{
    QW_INTERFACE_INIT(keyboard)

public:
    QW_INTERFACE(name)
    QW_INTERFACE(led_update)
};

QW_END_NAMESPACE
