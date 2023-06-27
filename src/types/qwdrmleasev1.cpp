// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdrmleasev1.h"

extern "C" {
#include <wlr/types/wlr_drm_lease_v1.h>
}

QW_BEGIN_NAMESPACE

wlr_drm_lease_v1 *QWDrmLeaseV1::handle() const
{
    return reinterpret_cast<wlr_drm_lease_v1*>(const_cast<QWDrmLeaseV1*>(this));
}

QWDrmLeaseV1 *QWDrmLeaseV1::from(wlr_drm_lease_v1 *handle)
{
    return reinterpret_cast<QWDrmLeaseV1*>(handle);
}

QW_END_NAMESPACE
