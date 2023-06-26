// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_data_device_manager;
struct wlr_data_source;

typedef uint32_t wl_data_device_manager_dnd_action_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QW_EXPORT QWDataSource
{
public:
    QWDataSource() = delete;
    ~QWDataSource() = delete;

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
class QW_EXPORT QWDataDeviceManager : public QObject, public QWObject
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

Q_SIGNALS:
    void beforeDestroy(QWDataDeviceManager *self);

private:
    QWDataDeviceManager(wlr_data_device_manager *handle, bool isOwner);
    ~QWDataDeviceManager() = default;
};

QW_END_NAMESPACE
