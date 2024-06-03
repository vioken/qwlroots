// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatacontrolv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_control_v1.h>
}

QW_BEGIN_NAMESPACE

class QWDataControlManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWDataControlManagerV1Private(wlr_data_control_manager_v1 *handle, bool isOwner, QWDataControlManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_device, this, &QWDataControlManagerV1Private::on_new_device);
    }

    void on_new_device(void *);

    QW_DECLARE_PUBLIC(QWDataControlManagerV1)
};

void QWDataControlManagerV1Private::on_new_device(void *data)
{
    auto *device = QWDataControlDeviceV1::from(reinterpret_cast<wlr_data_control_device_v1*>(data));
    Q_EMIT q_func()->newDevice(device);
}

QWDataControlManagerV1::QWDataControlManagerV1(wlr_data_control_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWDataControlManagerV1Private(handle, isOwner, this))
{

}

QWDataControlManagerV1 *QWDataControlManagerV1::get(wlr_data_control_manager_v1 *handle)
{
    return static_cast<QWDataControlManagerV1*>(QWDataControlManagerV1Private::map.value(handle));
}

QWDataControlManagerV1 *QWDataControlManagerV1::from(wlr_data_control_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDataControlManagerV1(handle, false);
}

QWDataControlManagerV1 *QWDataControlManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_data_control_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWDataControlManagerV1(handle, true);
}

QW_END_NAMESPACE
