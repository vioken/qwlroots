// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdrmleasev1.h"

extern "C" {
#include <wlr/types/wlr_drm_lease_v1.h>
}

QW_BEGIN_NAMESPACE

wlr_drm_lease_request_v1 *QWDrmLeaseRequestV1::handle() const
{
    return reinterpret_cast<wlr_drm_lease_request_v1*>(const_cast<QWDrmLeaseRequestV1*>(this));
}

QWDrmLeaseRequestV1 *QWDrmLeaseRequestV1::from(wlr_drm_lease_request_v1 *handle)
{
    return reinterpret_cast<QWDrmLeaseRequestV1*>(handle);
}

QWDrmLeaseV1 *QWDrmLeaseRequestV1::grant()
{
    auto *drmLease = wlr_drm_lease_request_v1_grant(handle());
    if (!drmLease)
        return nullptr;
    return QWDrmLeaseV1::from(drmLease);
}

void QWDrmLeaseRequestV1::reject()
{
    wlr_drm_lease_request_v1_reject(handle());
}

QW_END_NAMESPACE
