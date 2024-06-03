// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QAbstractEventDispatcher>
#include <QSocketNotifier>
#include <QThread>
#include <wayland-server.h>

QW_BEGIN_NAMESPACE

class QWDisplayPrivate : public QWWrapObjectPrivate
{
public:
    QWDisplayPrivate(wl_display *handle, bool isOwner, QWDisplay *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, nullptr,
                              toDestroyFunction(wl_display_destroy))
    {

    }
    ~QWDisplayPrivate() {
        Q_ASSERT(isHandleOwner);
        if (m_handle) {
            auto display = q_func()->handle();
            wl_display_destroy_clients(display);
        }
    }

    QW_DECLARE_PUBLIC(QWDisplay)
    wl_event_loop *loop = nullptr;
};

QWDisplay::QWDisplay(QObject *parent)
    : QWWrapObject(*new QWDisplayPrivate(wl_display_create(), true, this), parent)
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

wl_event_loop *QWDisplay::eventLoop() const
{
    return wl_display_get_event_loop(handle());
}

QW_END_NAMESPACE
