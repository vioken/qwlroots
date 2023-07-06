// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandserver.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/xwayland/server.h>
}

QW_BEGIN_NAMESPACE
class QWXWaylandServerPrivate : public QWObjectPrivate
{
public:
    QWXWaylandServerPrivate(wlr_xwayland_server *handle, bool isOwner, QWXWaylandServer *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXWaylandServerPrivate::on_destroy);
#if WLR_VERSION_MINOR > 16
        sc.connect(&handle->events.start, this, &QWXWaylandServerPrivate::on_start);
        sc.connect(&handle->events.ready, this, &QWXWaylandServerPrivate::on_ready);
#endif
    }
    ~QWXWaylandServerPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_xwayland_server_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
#if WLR_VERSION_MINOR > 16
    void on_start(void *);
    void on_ready(void *);
#endif

    static QHash<void*, QWXWaylandServer*> map;
    QW_DECLARE_PUBLIC(QWXWaylandServer)
    QWSignalConnector sc;
};
QHash<void*, QWXWaylandServer*> QWXWaylandServerPrivate::map;

void QWXWaylandServerPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

#if WLR_VERSION_MINOR > 16
void QWXWaylandServerPrivate::on_start(void *)
{
    Q_EMIT q_func()->start();
}

void QWXWaylandServerPrivate::on_ready(void *)
{
    Q_EMIT q_func()->ready();
}
#endif

QWXWaylandServer::QWXWaylandServer(wlr_xwayland_server *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXWaylandServerPrivate(handle, isOwner, this))
{

}

QWXWaylandServer *QWXWaylandServer::get(wlr_xwayland_server *handle)
{
    return QWXWaylandServerPrivate::map.value(handle);
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
