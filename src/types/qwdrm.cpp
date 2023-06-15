// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdrm.h"
#include "qwdisplay.h"
#include "qwrenderer.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wayland-server.h>
#include <wlr/types/wlr_drm.h>
}

QW_BEGIN_NAMESPACE

class QWDrmPrivate : public QWObjectPrivate
{
public:
    QWDrmPrivate(wlr_drm *handle, bool isOwner, QWDrm *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWDrmPrivate::on_destroy);
    }
    ~QWDrmPrivate() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWDrm*> map;
    QW_DECLARE_PUBLIC(QWDrm)
    QWSignalConnector sc;
};
QHash<void*, QWDrm*> QWDrmPrivate::map;

void QWDrmPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWDrm::QWDrm(wlr_drm *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWDrmPrivate(handle, isOwner, this))
{

}

QWDrm *QWDrm::get(wlr_drm *handle)
{
    return QWDrmPrivate::map.value(handle);
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
    auto *handle = wlr_drm_buffer_from_resource(resource);
    return handle ? from(handle) : nullptr;
}

bool QWDrmBuffer::isDrmBufferResource(wl_resource *resource)
{
    return wlr_drm_buffer_is_resource(resource);
}

QW_END_NAMESPACE
