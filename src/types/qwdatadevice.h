// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_data_device_manager;

QW_BEGIN_NAMESPACE

class QWDisplay;
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
