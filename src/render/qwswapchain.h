// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/render/swapchain.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(swapchain)
{
public:
    QW_FUNC_MEMBER(swapchain, destroy)
    QW_FUNC_MEMBER(swapchain, create)
    QW_FUNC_MEMBER(swapchain, acquire)
    QW_FUNC_MEMBER(swapchain, set_buffer_submitted)
};

QW_END_NAMESPACE
