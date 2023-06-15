// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwayland.h"

#include <qwdisplay.h>
#include <qwcompositor.h>
#include <qwseat.h>
#include <QImage>
#include <QPoint>

extern "C" {
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
}

QW_BEGIN_NAMESPACE

QWXWayland *QWXWayland::create(QWDisplay *wl_display, QWCompositor *compositor, bool lazy)
{
    auto* pointer = wlr_xwayland_create(wl_display->handle(), compositor->handle(), lazy);
    return pointer ? from(pointer) : nullptr;
}

QWXWayland* QWXWayland::from(wlr_xwayland* xwayland)
{
    return reinterpret_cast<QWXWayland*>(xwayland);
}

void QWXWayland::destroy()
{
    wlr_xwayland_destroy(handle());
}

void QWXWayland::setCursor(const QImage& image, const QPoint& hotspot)
{
    QImage img = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    wlr_xwayland_set_cursor(handle(),
                            img.bits(),
                            img.bytesPerLine(),
                            img.width(),
                            img.height(),
                            hotspot.x(),
                            hotspot.y());
}

void QWXWayland::setSeat(QWSeat *seat)
{
    wlr_xwayland_set_seat(handle(), seat->handle());
}

wlr_xwayland* QWXWayland::handle() const
{
    return reinterpret_cast<wlr_xwayland*>(const_cast<QWXWayland*>(this));
}

QW_END_NAMESPACE
