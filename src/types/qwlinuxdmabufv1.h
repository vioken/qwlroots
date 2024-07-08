// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_linux_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(linux_dmabuf_feedback_v1)
{
public:
    // Empty parameters
    QW_FUNC_MEMBER(linux_dmabuf_feedback_v1, finish)
    // const wlr_linux_dmabuf_feedback_v1_init_options *options
    QW_FUNC_MEMBER(linux_dmabuf_feedback_v1, init_with_options)
};

class QW_CLASS_OBJECT(linux_dmabuf_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wl_display *display, uint32_t version, const wlr_linux_dmabuf_feedback_v1 *default_feedback
    QW_FUNC_STATIC(linux_dmabuf_v1, create)
};

QW_END_NAMESPACE
