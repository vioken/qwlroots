// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlinuxdmabufv1.h"

extern "C" {
#include <wlr/types/wlr_linux_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

wlr_linux_dmabuf_feedback_v1 *QWLinuxDmabufFeedbackV1::handle() const
{
    return reinterpret_cast<wlr_linux_dmabuf_feedback_v1*>(const_cast<QWLinuxDmabufFeedbackV1*>(this));
}

QWLinuxDmabufFeedbackV1 *QWLinuxDmabufFeedbackV1::from(wlr_linux_dmabuf_feedback_v1 *handle)
{
    return reinterpret_cast<QWLinuxDmabufFeedbackV1*>(handle);
}

#if WLR_VERSION_MINOR > 16

void QWLinuxDmabufFeedbackV1::finish()
{
    wlr_linux_dmabuf_feedback_v1_finish(handle());
}

bool QWLinuxDmabufFeedbackV1::initWithOptions(const wlr_linux_dmabuf_feedback_v1_init_options *options)
{
    return wlr_linux_dmabuf_feedback_v1_init_with_options(handle(), options);
}

#endif // WLR_VERSION_MINOR > 16

QW_END_NAMESPACE
