// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxwaylandserver.h"

#include <qwdisplay.h>

extern "C" {
#include <wlr/xwayland/server.h>
}

QW_BEGIN_NAMESPACE

QWXWaylandServer* QWXWaylandServer::from(wlr_xwayland_server* xwayland)
{
    return reinterpret_cast<QWXWaylandServer*>(xwayland);
}

QWXWaylandServer* QWXWaylandServer::create(QWDisplay *display, wlr_xwayland_server_options *options)
{
    return from(wlr_xwayland_server_create(display->handle(), options));
}

void QWXWaylandServer::destroy()
{
    wlr_xwayland_server_destroy(handle());
}

wlr_xwayland_server* QWXWaylandServer::handle() const
{
    return reinterpret_cast<wlr_xwayland_server*>(const_cast<QWXWaylandServer*>(this));
}

QW_END_NAMESPACE
