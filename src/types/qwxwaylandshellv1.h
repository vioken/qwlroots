// Copyright (C) 2023 JiDe Zhang <zhangjide@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

QT_BEGIN_NAMESPACE

class QImage;
class QPoint;

QT_END_NAMESPACE

struct wlr_xwayland_shell_v1;
struct wl_client;
struct wlr_xwayland_surface;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWDisplay;
class QWXWaylandShellV1Private;
class QW_EXPORT QWXWaylandShellV1 : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXWaylandShellV1)

public:
    static QWXWaylandShellV1 *create(QWDisplay *display, uint32_t version);
    static QWXWaylandShellV1 *get(wlr_xwayland_shell_v1 *handle);

    wlr_xwayland_shell_v1 *handle() const;

    void setClient(wl_client *client);

    QWSurface *surfaceFromSerial(uint64_t serial) const;

Q_SIGNALS:
    void newSurface(wlr_xwayland_surface *surface);

private:
    explicit QWXWaylandShellV1(wlr_xwayland_shell_v1 *handle, QWDisplay *parent);
    ~QWXWaylandShellV1() = default;
};

QW_END_NAMESPACE
