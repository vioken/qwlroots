// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qw_global.h>
#include <QObject>

QT_BEGIN_NAMESPACE
class QImage;
class QPoint;
QT_END_NAMESPACE

struct wlr_xwayland;
struct wlr_xwayland_surface;
struct wlr_xwayland_remove_startup_info_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWCompositor;
class QWSeat;
class QWXWaylandServer;
class QWXWaylandShellV1;
class QWXWaylandPrivate;
class QW_EXPORT QWXWayland : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXWayland)
public:
    static QWXWayland *create(QWDisplay *display, QWCompositor *compositor, bool lazy);
    static QWXWayland *get(wlr_xwayland *handle);
    wlr_xwayland *handle() const;

    void setCursor(const QImage &image, const QPoint &hotspot);
    void setSeat(QWSeat *seat);

Q_SIGNALS:
    void ready();
    void newSurface(wlr_xwayland_surface *surface);
    void removeStartupInfo(wlr_xwayland_remove_startup_info_event *event);

private:
    explicit QWXWayland(wlr_xwayland *handle, QWXWaylandServer *parent);
    ~QWXWayland() = default;
};

QW_END_NAMESPACE
