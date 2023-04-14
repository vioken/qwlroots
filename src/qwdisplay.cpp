// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QAbstractEventDispatcher>
#include <QSocketNotifier>
#include <QThread>
#include <wayland-server.h>

QW_BEGIN_NAMESPACE

class QWDisplayPrivate : public QWObjectPrivate
{
public:
    QWDisplayPrivate(wl_display *handle, bool isOwner, QWDisplay *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {

    }
    ~QWDisplayPrivate() {
        Q_EMIT q_func()->beforeDestroy(q_func());

        sc.invalidate();
        Q_ASSERT(isHandleOwner);
        if (m_handle) {
            auto display = q_func()->handle();
            wl_display_destroy_clients(display);
            wl_display_destroy(display);
        }
    }

    QW_DECLARE_PUBLIC(QWDisplay)
    QWSignalConnector sc;
    wl_event_loop *loop = nullptr;
};

QWDisplay::QWDisplay(QObject *parent)
    : QObject(parent)
    , QWObject(*new QWDisplayPrivate(wl_display_create(), true, this))
{

}

int QWDisplay::addSocket(const char *name)
{
    return wl_display_add_socket(handle(), name);
}

int QWDisplay::addSocketFd(int fd)
{
    return wl_display_add_socket_fd(handle(), fd);
}

const char *QWDisplay::addSocketAuto()
{
    return wl_display_add_socket_auto(handle());
}

void QWDisplay::exec()
{
    wl_display_run(handle());
}

void QWDisplay::start(QThread *thread)
{
    d_func()->loop = wl_display_get_event_loop(handle());
    int fd = wl_event_loop_get_fd(d_func()->loop);

    auto processWaylandEvents = [this] () {
        int ret = wl_event_loop_dispatch(d_func()->loop, 0);
        if (ret)
            fprintf(stderr, "wl_event_loop_dispatch error: %d\n", ret);
        wl_display_flush_clients(handle());
    };

    auto sockNot = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(sockNot, &QSocketNotifier::activated, this, processWaylandEvents);

    QAbstractEventDispatcher *dispatcher = thread->eventDispatcher();
    connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, this, processWaylandEvents);
}

void QWDisplay::terminate()
{
    wl_display_terminate(handle());
}

QW_END_NAMESPACE
