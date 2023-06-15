// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_xwayland_server;
struct wlr_xwayland_server_options;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXWaylandServer
{
public:
    QWXWaylandServer() = delete;
    ~QWXWaylandServer() = delete;

    static QWXWaylandServer* from(wlr_xwayland_server* xwayland);
    static QWXWaylandServer* create(QWDisplay *display, wlr_xwayland_server_options *options);

    void destroy();
    wlr_xwayland_server* handle() const;
};

QW_END_NAMESPACE

