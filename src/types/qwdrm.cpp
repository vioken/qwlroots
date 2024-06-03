// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdrm.h"
#include "qwdisplay.h"
#include "qwrenderer.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wayland-server.h>
#include <wlr/types/wlr_drm.h>
}

QW_BEGIN_NAMESPACE

class QWDrmPrivate : public QWWrapObjectPrivate
{
public:
    QWDrmPrivate(wlr_drm *handle, bool isOwner, QWDrm *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWDrm)
};

QWDrm::QWDrm(wlr_drm *handle, bool isOwner)
    : QWWrapObject(*new QWDrmPrivate(handle, isOwner, this))
{

}

QWDrm *QWDrm::get(wlr_drm *handle)
{
    return static_cast<QWDrm*>(QWDrmPrivate::map.value(handle));
}

QWDrm *QWDrm::from(wlr_drm *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDrm(handle, false);
}

QWDrm *QWDrm::create(QWDisplay *display, QWRenderer *render)
{
    auto *handle = wlr_drm_create(display->handle(), render->handle());
    if (!handle)
        return nullptr;
    return new QWDrm(handle, true);
}

wlr_drm_buffer *QWDrmBuffer::handle() const
{
    return reinterpret_cast<wlr_drm_buffer*>(const_cast<QWDrmBuffer*>(this));
}

QWDrmBuffer *QWDrmBuffer::from(wlr_drm_buffer *handle)
{
    return reinterpret_cast<QWDrmBuffer*>(handle);
}

QWDrmBuffer *QWDrmBuffer::from(wl_resource *resource)
{
    auto *handle = wlr_drm_buffer_try_from_resource(resource);
    return handle ? from(handle) : nullptr;
}

QW_END_NAMESPACE
