// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_fullscreen_shell_v1;
struct wlr_fullscreen_shell_v1_present_surface_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWFullScreenShellV1Private;
class QW_EXPORT QWFullScreenShellV1: public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWFullScreenShellV1)
public:
    inline wlr_fullscreen_shell_v1 *handle() const {
        return QWObject::handle<wlr_fullscreen_shell_v1>();
    }

    static QWFullScreenShellV1 *get(wlr_fullscreen_shell_v1 *handle);
    static QWFullScreenShellV1 *from(wlr_fullscreen_shell_v1 *handle);
    static QWFullScreenShellV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWFullScreenShellV1 *self);
    void presentSurface(wlr_fullscreen_shell_v1_present_surface_event *event);

private:
    QWFullScreenShellV1(wlr_fullscreen_shell_v1 *handle, bool isOwner);
    ~QWFullScreenShellV1() = default;
};

QW_END_NAMESPACE

