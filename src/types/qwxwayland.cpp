// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwayland.h"

#include "qwxwaylandserver.h"
#include "util/qwsignalconnector.h"
#include <qwdisplay.h>
#include <qwcompositor.h>
#include <qwseat.h>
#include <QImage>
#include <QPoint>
#include <QHash>

extern "C" {
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
}

QW_BEGIN_NAMESPACE

class QWXWaylandPrivate : public QWObjectPrivate
{
public:
    QWXWaylandPrivate(wlr_xwayland *handle, bool isOwner, QWXWayland *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.ready, q_func(), &QWXWayland::ready);
        sc.connect(&handle->events.new_surface, q_func(), &QWXWayland::newSurface);
        sc.connect(&handle->events.remove_startup_info, q_func(), &QWXWayland::removeStartupInfo);
    }

    ~QWXWaylandPrivate() {
        if (!m_handle) {
            return;
        }
        destroy();
        if (isHandleOwner) {
            wlr_xwayland_destroy(q_func()->handle());
        }
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    static QHash<void*, QWXWayland*> map;
    QW_DECLARE_PUBLIC(QWXWayland)
    QWSignalConnector sc;
};
QHash<void*, QWXWayland*> QWXWaylandPrivate::map;

QWXWayland *QWXWayland::create(QWDisplay *wl_display, QWCompositor *compositor, bool lazy)
{
    auto *handle = wlr_xwayland_create(wl_display->handle(), compositor->handle(), lazy);
    if (!handle)
        return nullptr;
    auto *parent = QWXWaylandServer::from(handle->server);
    return new QWXWayland(handle, true, parent);
}

QWXWayland *QWXWayland::get(wlr_xwayland *handle)
{
    return QWXWaylandPrivate::map.value(handle);
}

void QWXWayland::setCursor(const QImage &image, const QPoint &hotspot)
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

wlr_xwayland *QWXWayland::handle() const
{
    return QWObject::handle<wlr_xwayland>();
}

QWXWayland::QWXWayland(wlr_xwayland *handle, bool isOwner, QWXWaylandServer *parent)
    : QObject(parent)
    , QWObject(*new QWXWaylandPrivate(handle, isOwner, this))
{
}

QW_END_NAMESPACE
