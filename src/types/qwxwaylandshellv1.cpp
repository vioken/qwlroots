// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandshellv1.h"

#include <qwglobal.h>
#include <qwdisplay.h>
#include <qwcompositor.h>
#include <util/qwsignalconnector.h>

#include <QHash>

extern "C" {
#include <wlr/xwayland/shell.h>
}

QW_BEGIN_NAMESPACE

class QWXWaylandShellV1Private : public QWObjectPrivate {
public:
    QWXWaylandShellV1Private(wlr_xwayland_shell_v1* handle, bool isOwner, QWXWaylandShellV1* qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_surface, q_func(), &QWXWaylandShellV1::newSurface);
    }

    ~QWXWaylandShellV1Private() {
        if (!m_handle) {
            return;
        }
        destroy();
        if (isHandleOwner) {
            wlr_xwayland_shell_v1_destroy(q_func()->handle());
        }
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    static QHash<void*, QWXWaylandShellV1*> map;
    QW_DECLARE_PUBLIC(QWXWaylandShellV1)
    QWSignalConnector sc;
};
QHash<void*, QWXWaylandShellV1*> QWXWaylandShellV1Private::map;

QWXWaylandShellV1* QWXWaylandShellV1::get(wlr_xwayland_shell_v1* handle)
{
    return QWXWaylandShellV1Private::map.value(handle);
}

QWXWaylandShellV1* QWXWaylandShellV1::create(QWDisplay* display, uint32_t version)
{
    auto* handle = wlr_xwayland_shell_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWXWaylandShellV1(handle, true, display);
}

wlr_xwayland_shell_v1* QWXWaylandShellV1::handle() const
{
    return QWObject::handle<wlr_xwayland_shell_v1>();
}

void QWXWaylandShellV1::setClient(wl_client* client)
{
    wlr_xwayland_shell_v1_set_client(handle(), client);
}

QWSurface *QWXWaylandShellV1::surfaceFromSerial(uint64_t serial) const
{
    auto* surface = wlr_xwayland_shell_v1_surface_from_serial(handle(), serial);
    return surface ? QWSurface::from(surface) : nullptr;
}

QWXWaylandShellV1::QWXWaylandShellV1(wlr_xwayland_shell_v1* handle, bool isOwner, QWDisplay* parent)
    : QObject(parent)
    , QWObject(*new QWXWaylandShellV1Private(handle, isOwner, this))
{
}

QW_END_NAMESPACE
