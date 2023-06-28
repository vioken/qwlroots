// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsinglepixelbufferv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_single_pixel_buffer_v1.h>
}

QW_BEGIN_NAMESPACE

wlr_single_pixel_buffer_manager_v1 *QWSinglePixelBufferManagerV1::handle() const
{
    return reinterpret_cast<wlr_single_pixel_buffer_manager_v1*>(const_cast<QWSinglePixelBufferManagerV1*>(this));
}

QWSinglePixelBufferManagerV1 *QWSinglePixelBufferManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_single_pixel_buffer_manager_v1_create(display->handle());
    return from(handle);
}

QWSinglePixelBufferManagerV1 *QWSinglePixelBufferManagerV1::from(wlr_single_pixel_buffer_manager_v1 *handle)
{
    return reinterpret_cast<QWSinglePixelBufferManagerV1*>(handle);
}

QW_END_NAMESPACE
