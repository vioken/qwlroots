// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputmanagementv1.h"

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

void QWOutputConfigurationV1::operator delete(QWOutputConfigurationV1 *p, std::destroying_delete_t)
{
    wlr_output_configuration_v1_destroy(p->handle());
}

wlr_output_configuration_v1 *QWOutputConfigurationV1::handle() const
{
    return reinterpret_cast<wlr_output_configuration_v1*>(const_cast<QWOutputConfigurationV1*>(this));
}

QWOutputConfigurationV1 *QWOutputConfigurationV1::from(wlr_output_configuration_v1 *handle)
{
    return reinterpret_cast<QWOutputConfigurationV1*>(handle);
}

QWOutputConfigurationV1 *QWOutputConfigurationV1::create()
{
    return from(wlr_output_configuration_v1_create());
}

void QWOutputConfigurationV1::sendFailed()
{
    wlr_output_configuration_v1_send_failed(handle());
}

void QWOutputConfigurationV1::sendSucceeded()
{
    wlr_output_configuration_v1_send_succeeded(handle());
}

QW_END_NAMESPACE
