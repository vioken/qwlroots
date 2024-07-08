// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/types/wlr_xcursor_manager.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(xcursor_manager)
{
public:
    // const char *name, uint32_t size
    QW_FUNC_STATIC(xcursor_manager, create)

    // Empty parameters
    QW_FUNC_MEMBER(xcursor_manager, destroy)
    // float scale
    QW_FUNC_MEMBER(xcursor_manager, load)
    // const char *name, float scale
    QW_FUNC_MEMBER(xcursor_manager, get_xcursor)
};

QW_END_NAMESPACE
