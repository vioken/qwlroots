// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_data_device_manager;
struct wlr_data_source;
struct wlr_drag;
struct wlr_drag_motion_event;
struct wlr_drag_drop_event;

typedef uint32_t wl_data_device_manager_dnd_action_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QW_EXPORT QWDataSource
{
public:
    QWDataSource() = delete;
    QW_DISALLOW_DESTRUCTOR(QWDataSource)

    void operator delete(QWDataSource *p, std::destroying_delete_t);

    wlr_data_source *handle() const;

    static QWDataSource *from(wlr_data_source *handle);

    void accept(uint32_t serial, const char *mime_type);
    void dndAction(wl_data_device_manager_dnd_action_t action);
    void dndDrop();
    void dndFinish();
    void send(const char *mime_type, int32_t fd);
};

class QWDataDeviceManagerPrivate;
class QW_EXPORT QWDataDeviceManager : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDataDeviceManager)
public:
    inline wlr_data_device_manager *handle() const {
        return QWObject::handle<wlr_data_device_manager>();
    }

    static QWDataDeviceManager *get(wlr_data_device_manager *handle);
    static QWDataDeviceManager *from(wlr_data_device_manager *handle);
    static QWDataDeviceManager *create(QWDisplay *display);

private:
    QWDataDeviceManager(wlr_data_device_manager *handle, bool isOwner);
    ~QWDataDeviceManager() = default;
};

class QWDragPrivate;
class QW_EXPORT QWDrag : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDrag)
public:
    inline wlr_drag *handle() const {
        return QWObject::handle<wlr_drag>();
    }

    static QWDrag *get(wlr_drag *handle);
    static QWDrag *from(wlr_drag *handle);

Q_SIGNALS:
    void focus();
    void motion(wlr_drag_motion_event *event);
    void drop(wlr_drag_drop_event *event);

private:
    QWDrag(wlr_drag *handle, bool isOwner);
    ~QWDrag() = default;
};

QW_END_NAMESPACE
