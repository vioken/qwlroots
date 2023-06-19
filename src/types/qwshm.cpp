// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwshm.h"
#include "qwrenderer.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_shm.h>
}

QW_BEGIN_NAMESPACE

wlr_shm *QWShm::handle() const
{
    return reinterpret_cast<wlr_shm*>(const_cast<QWShm*>(this));
}

QWShm *QWShm::from(wlr_shm *handle)
{
    return reinterpret_cast<QWShm*>(handle);
}

QWShm *QWShm::create(wl_display *display, uint32_t version, const uint32_t *formats, size_t formatsLen)
{
    auto *handle = wlr_shm_create(display, version, formats, formatsLen);
    return from(handle);
}

QWShm *QWShm::create(wl_display *display, uint32_t version, QWRenderer *renderer)
{
    auto *handle = wlr_shm_create_with_renderer(display, version, renderer->handle());
    return from(handle);
}

QW_END_NAMESPACE
