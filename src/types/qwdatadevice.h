// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_data_device_manager;

QW_BEGIN_NAMESPACE

class QWDataDeviceManagerPrivate;
class QW_EXPORT QWDataDeviceManager : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWDataDeviceManager)
public:
    explicit QWDataDeviceManager(wlr_data_device_manager *handle);

    static QWDataDeviceManager *create(wl_display *display);

    inline wlr_data_device_manager *handle() const {
        return QWObject::handle<wlr_data_device_manager>();
    }

private:
    ~QWDataDeviceManager() = default;
    using QObject::deleteLater;
};

QW_END_NAMESPACE
