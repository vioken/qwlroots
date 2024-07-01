// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

#include <QAbstractEventDispatcher>
#include <QSocketNotifier>
#include <QThread>

#include <wayland-server.h>

struct wl_event_loop;
QW_BEGIN_NAMESPACE

class qw_display : public qw_object<wl_display, qw_display>
{
    Q_OBJECT
    using qw_object::qw_object;
    friend class qw_object;

#undef QW_FUNC_MEMBER
#define QW_FUNC_MEMBER(wl_type_suffix, wl_func_suffix) \
    template<typename ...Args> \
        QW_ALWAYS_INLINE auto wl_func_suffix(Args &&... args) { \
            return wl_##wl_type_suffix##_##wl_func_suffix(*this, std::forward<Args>(args)...); \
    }

public:
    QW_FUNC_MEMBER(display, add_socket)
    QW_FUNC_MEMBER(display, socket_fd)
    QW_FUNC_MEMBER(display, socket_auto)
    QW_FUNC_MEMBER(display, run)
    QW_FUNC_MEMBER(display, terminate)
    QW_FUNC_MEMBER(display, get_event_loop)

    wl_event_loop *eventLoop() const;

    void start(QThread *thread) {
        auto loop = wl_display_get_event_loop(*this);
        int fd = wl_event_loop_get_fd(loop);

        auto processWaylandEvents = [this, loop] () {
            int ret = wl_event_loop_dispatch(loop, 0);
            if (ret)
                fprintf(stderr, "wl_event_loop_dispatch error: %d\n", ret);
            wl_display_flush_clients(*this);
        };

        auto sockNot = new QSocketNotifier(fd, QSocketNotifier::Read, this);
        connect(sockNot, &QSocketNotifier::activated, this, processWaylandEvents);

        QAbstractEventDispatcher *dispatcher = thread->eventDispatcher();
        connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, this, processWaylandEvents);
    }
};

QW_END_NAMESPACE
