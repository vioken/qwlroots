// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_single_pixel_buffer_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QW_EXPORT QWSinglePixelBufferManagerV1
{
public:
    QWSinglePixelBufferManagerV1() = delete;
    ~QWSinglePixelBufferManagerV1() = delete;

    wlr_single_pixel_buffer_manager_v1 *handle() const;
    static QWSinglePixelBufferManagerV1 *from(wlr_single_pixel_buffer_manager_v1 *handle);
    static QWSinglePixelBufferManagerV1 *create(QWDisplay *display);
};

QW_END_NAMESPACE
