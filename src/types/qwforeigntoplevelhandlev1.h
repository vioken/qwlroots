// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_foreign_toplevel_handle_v1;
struct wlr_foreign_toplevel_handle_v1_maximized_event;
struct wlr_foreign_toplevel_handle_v1_minimized_event;
struct wlr_foreign_toplevel_handle_v1_activated_event;
struct wlr_foreign_toplevel_handle_v1_fullscreen_event;
struct wlr_foreign_toplevel_handle_v1_set_rectangle_event;
struct wlr_foreign_toplevel_manager_v1;

QW_BEGIN_NAMESPACE

class QWOutput;
class QWDisplay;
class QWForeignToplevelManagerV1Private;
class QW_EXPORT QWForeignToplevelManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWForeignToplevelManagerV1)
public:
    inline wlr_foreign_toplevel_manager_v1 *handle() const {
        return QWObject::handle<wlr_foreign_toplevel_manager_v1>();
    }

    static QWForeignToplevelManagerV1 *get(wlr_foreign_toplevel_manager_v1 *handle);
    static QWForeignToplevelManagerV1 *from(wlr_foreign_toplevel_manager_v1 *handle);
    static QWForeignToplevelManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWForeignToplevelManagerV1 *self);

private:
    QWForeignToplevelManagerV1(wlr_foreign_toplevel_manager_v1 *handle, bool isOwner);
    ~QWForeignToplevelManagerV1() = default;
};

class QWForeignToplevelHandleV1Private;
class QW_EXPORT QWForeignToplevelHandleV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWForeignToplevelHandleV1)
public:
    ~QWForeignToplevelHandleV1() = default;

    inline wlr_foreign_toplevel_handle_v1 *handle() const {
        return QWObject::handle<wlr_foreign_toplevel_handle_v1>();
    }

    static QWForeignToplevelHandleV1 *get(wlr_foreign_toplevel_handle_v1 *handle);
    static QWForeignToplevelHandleV1 *from(wlr_foreign_toplevel_handle_v1 *handle);
    static QWForeignToplevelHandleV1 *create(QWForeignToplevelManagerV1 *manager);

    void outputEnter(QWOutput *output);
    void outputLeave(QWOutput *output);
    void setActivated(bool activated);
    void setAppId(const char *appId);
    void setFullScreen(bool fullScreen);
    void setMaximized(bool maximized);
    void setMinimized(bool minimized);
    void setParent(QWForeignToplevelHandleV1 *parent);
    void setTitle(const char *title);

Q_SIGNALS:
    void beforeDestroy(QWForeignToplevelHandleV1 *self);
    void requestMaximize(wlr_foreign_toplevel_handle_v1_maximized_event *event);
    void requestMinimize(wlr_foreign_toplevel_handle_v1_minimized_event *event);
    void requestActivate(wlr_foreign_toplevel_handle_v1_activated_event *event);
    void requestFullscreen(wlr_foreign_toplevel_handle_v1_fullscreen_event *event);
    void requestClose();
    void rectangleChanged(wlr_foreign_toplevel_handle_v1_set_rectangle_event *event);

private:
    QWForeignToplevelHandleV1(wlr_foreign_toplevel_handle_v1 *handle, bool isOwner);
};

QW_END_NAMESPACE

