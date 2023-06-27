// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputmanagementv1.h"
#include "qwoutput.h"

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

wlr_output_configuration_head_v1 *QWOutputConfigurationHeadV1::handle() const
{
    return reinterpret_cast<wlr_output_configuration_head_v1*>(const_cast<QWOutputConfigurationHeadV1*>(this));
}

QWOutputConfigurationHeadV1 *QWOutputConfigurationHeadV1::from(wlr_output_configuration_head_v1 *handle)
{
    return reinterpret_cast<QWOutputConfigurationHeadV1*>(handle);
}

QWOutputConfigurationHeadV1 *QWOutputConfigurationHeadV1::create(QWOutputConfigurationV1 *config, QWOutput *output)
{
    return from(wlr_output_configuration_head_v1_create(config->handle(), output->handle()));
}

QW_END_NAMESPACE
