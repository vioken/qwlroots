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

#define QW_DISPLAY_FUNC_MEMBER(wl_func_suffix, ret_type, ...) \
    template<typename ...Args> \
    QW_ALWAYS_INLINE ret_type \
    wl_func_suffix(Args &&... args) const requires std::is_invocable_v<void(*)(__VA_ARGS__), Args...> \
    { \
        static_assert(std::is_invocable_v<decltype(wl_display##_##wl_func_suffix), decltype(*this), Args...>, ""); \
        return wl_display##_##wl_func_suffix(*this, std::forward<Args>(args)...); \
    }

public:
    explicit qw_display() : qw_object(wl_display_create(), true) { }

    QW_DISPLAY_FUNC_MEMBER(add_socket, int, const char *name)
    QW_DISPLAY_FUNC_MEMBER(add_socket_fd, int, int sock_fd)
    QW_DISPLAY_FUNC_MEMBER(add_socket_auto, const char*)
    QW_DISPLAY_FUNC_MEMBER(run, void)
    QW_DISPLAY_FUNC_MEMBER(terminate, void)
    QW_DISPLAY_FUNC_MEMBER(get_event_loop, wl_event_loop*)

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

protected:
    QW_DISPLAY_FUNC_MEMBER(destroy, void)
};

QW_END_NAMESPACE
