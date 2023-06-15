// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

class QImage;
class QPoint;
struct wlr_xwayland;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWCompositor;
class QWSeat;
class QWXWayland
{
public:
    QWXWayland() = delete;
    ~QWXWayland() = delete;

    static QWXWayland *create(QWDisplay *wl_display, QWCompositor *compositor, bool lazy);
    static QWXWayland* from(wlr_xwayland* xwayland);
    wlr_xwayland* handle() const;

    void destroy();
    void setCursor(const QImage& image, const QPoint& hotspot);
    void setSeat(QWSeat *seat);
};

QW_END_NAMESPACE
