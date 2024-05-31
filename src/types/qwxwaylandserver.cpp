// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandserver.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/xwayland/server.h>
}

QW_BEGIN_NAMESPACE
class QWXWaylandServerPrivate : public QWWrapObjectPrivate
{
public:
    QWXWaylandServerPrivate(wlr_xwayland_server *handle, bool isOwner, QWXWaylandServer *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy,
                              toDestroyFunction(wlr_xwayland_server_destroy))
    {
        sc.connect(&handle->events.start, this, &QWXWaylandServerPrivate::on_start);
        sc.connect(&handle->events.ready, this, &QWXWaylandServerPrivate::on_ready);
    }

    void on_start(void *);
    void on_ready(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXWaylandServer)
};
QHash<void*, QWWrapObject*> QWXWaylandServerPrivate::map;

void QWXWaylandServerPrivate::on_start(void *)
{
    Q_EMIT q_func()->start();
}

void QWXWaylandServerPrivate::on_ready(void *)
{
    Q_EMIT q_func()->ready();
}

QWXWaylandServer::QWXWaylandServer(wlr_xwayland_server *handle, bool isOwner)
    : QWWrapObject(*new QWXWaylandServerPrivate(handle, isOwner, this))
{

}

QWXWaylandServer *QWXWaylandServer::get(wlr_xwayland_server *handle)
{
    return static_cast<QWXWaylandServer*>(QWXWaylandServerPrivate::map.value(handle));
}

QWXWaylandServer *QWXWaylandServer::from(wlr_xwayland_server *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXWaylandServer(handle, false);
}

QWXWaylandServer *QWXWaylandServer::create(QWDisplay *display, wlr_xwayland_server_options *options)
{
    auto *handle = wlr_xwayland_server_create(display->handle(), options);
    return handle ? new QWXWaylandServer(handle, true) : nullptr;
}

QW_END_NAMESPACE
