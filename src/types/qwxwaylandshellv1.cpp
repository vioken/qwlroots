// Copyright (C) 2023 JiDe Zhang <zhangjide@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandshellv1.h"

#include "qwglobal.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include <private/qwglobal_p.h>

#include <QHash>

extern "C" {
#include <wlr/xwayland/shell.h>
}

QW_BEGIN_NAMESPACE

class QWXWaylandShellV1Private : public QWWrapObjectPrivate {
public:
    QWXWaylandShellV1Private(wlr_xwayland_shell_v1* handle, QWXWaylandShellV1* qq)
        : QWWrapObjectPrivate(handle, false, qq, &map)
    {
        sc.connect(&handle->events.new_surface, this, &QWXWaylandShellV1Private::on_new_surface);
    }

    ~QWXWaylandShellV1Private() {
        if (!m_handle)
            return;
        // Destroy following wl_display.
        Q_ASSERT(!isHandleOwner);
    }

    void on_new_surface(wlr_xwayland_surface *surface);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXWaylandShellV1)
};
QHash<void*, QWWrapObject*> QWXWaylandShellV1Private::map;

void QWXWaylandShellV1Private::on_new_surface(wlr_xwayland_surface *surface)
{
    Q_EMIT q_func()->newSurface(surface);
}

QWXWaylandShellV1* QWXWaylandShellV1::get(wlr_xwayland_shell_v1* handle)
{
    return static_cast<QWXWaylandShellV1*>(QWXWaylandShellV1Private::map.value(handle));
}

QWXWaylandShellV1* QWXWaylandShellV1::create(QWDisplay* display, uint32_t version)
{
    auto* handle = wlr_xwayland_shell_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWXWaylandShellV1(handle, display);
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

QWXWaylandShellV1::QWXWaylandShellV1(wlr_xwayland_shell_v1* handle, QWDisplay* parent)
    : QWWrapObject(*new QWXWaylandShellV1Private(handle, this), parent)
{
}

QW_END_NAMESPACE
