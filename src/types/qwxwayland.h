// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

class QImage;
class QPoint;
struct wlr_xwayland;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWCompositor;
class QWSeat;
class QWXWaylandServer;
class QWXWaylandPrivate;
class QW_EXPORT QWXWayland : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXWayland)
public:
    explicit QWXWayland(wlr_xwayland *handle, bool isOwner, QWXWaylandServer *parent);
    ~QWXWayland() = default;

    static QWXWayland *create(QWDisplay *display, QWCompositor *compositor, bool lazy);
    static QWXWayland *get(wlr_xwayland *handle);
    wlr_xwayland *handle() const;

    void setCursor(const QImage &image, const QPoint &hotspot);
    void setSeat(QWSeat *seat);

Q_SIGNALS:
    void beforeDestroy(QWXWayland *self);
    void ready();
    void newSurface();
    void removeStartupInfo();
};

QW_END_NAMESPACE
