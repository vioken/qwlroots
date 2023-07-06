// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_xwayland_server;
struct wlr_xwayland_server_options;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXWaylandServerPrivate;
class QW_EXPORT QWXWaylandServer : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXWaylandServer)
public:
    ~QWXWaylandServer() override = default;

    inline wlr_xwayland_server *handle() const {
        return QWObject::handle<wlr_xwayland_server>();
    }

    static QWXWaylandServer *get(wlr_xwayland_server *handle);
    static QWXWaylandServer *from(wlr_xwayland_server *handle);
    static QWXWaylandServer *create(QWDisplay *display, wlr_xwayland_server_options *options);

Q_SIGNALS:
    void beforeDestroy(QWXWaylandServer *self);
#if WLR_VERSION_MINOR > 16
    void start();
    void ready();
#endif

private:
    QWXWaylandServer(wlr_xwayland_server *handle, bool isOwner);
};

QW_END_NAMESPACE
