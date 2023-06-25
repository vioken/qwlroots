// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatacontrolv1.h"

extern "C" {
#include <wlr/types/wlr_data_control_v1.h>
}

QW_BEGIN_NAMESPACE

void QWDataControlDeviceV1::operator delete(QWDataControlDeviceV1 *p, std::destroying_delete_t)
{
    wlr_data_control_device_v1_destroy(p->handle());
}

wlr_data_control_device_v1 *QWDataControlDeviceV1::handle() const
{
    return reinterpret_cast<wlr_data_control_device_v1*>(const_cast<QWDataControlDeviceV1*>(this));
}

QWDataControlDeviceV1 *QWDataControlDeviceV1::from(wlr_data_control_device_v1 *handle)
{
    return reinterpret_cast<QWDataControlDeviceV1*>(handle);
}

QW_END_NAMESPACE
