// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_data_control_manager_v1;
struct wlr_data_control_device_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QW_EXPORT QWDataControlDeviceV1
{
public:
    QWDataControlDeviceV1() = delete;
    ~QWDataControlDeviceV1() = delete;

    void operator delete(QWDataControlDeviceV1 *p, std::destroying_delete_t);

    wlr_data_control_device_v1 *handle() const;

    static QWDataControlDeviceV1 *from(wlr_data_control_device_v1 *handle);
};

class QWDataControlManagerV1Private;
class QW_EXPORT QWDataControlManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDataControlManagerV1)
public:
    inline wlr_data_control_manager_v1 *handle() const {
        return QWObject::handle<wlr_data_control_manager_v1>();
    }

    static QWDataControlManagerV1 *get(wlr_data_control_manager_v1 *handle);
    static QWDataControlManagerV1 *from(wlr_data_control_manager_v1 *handle);
    static QWDataControlManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWDataControlManagerV1 *self);
    void newDevice(QWDataControlDeviceV1 *device);

private:
    QWDataControlManagerV1(wlr_data_control_manager_v1 *handle, bool isOwner);
    ~QWDataControlManagerV1() = default;
};

QW_END_NAMESPACE

