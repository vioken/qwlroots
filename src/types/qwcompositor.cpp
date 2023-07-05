// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcompositor.h"
#include "qwdisplay.h"
#include "qwtexture.h"
#include "qwoutput.h"
#include "util/qwsignalconnector.h"
#include "render/qwrenderer.h"

#include <QHash>
#include <QRectF>
#include <QPointF>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_compositor.h>
#undef static
}

static_assert(std::is_same_v<wl_output_transform_t, std::underlying_type_t<wl_output_transform>>);

QW_BEGIN_NAMESPACE

class QWCompositorPrivate : public QWObjectPrivate
{
public:
    QWCompositorPrivate(wlr_compositor *handle, bool isOwner, QWCompositor *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWCompositorPrivate::on_destroy);
        sc.connect(&handle->events.new_surface, this, &QWCompositorPrivate::on_new_surface);
    }
    ~QWCompositorPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWCompositor(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(void *data);

    static QHash<void*, QWCompositor*> map;
    QW_DECLARE_PUBLIC(QWCompositor)
    QWSignalConnector sc;
};
QHash<void*, QWCompositor*> QWCompositorPrivate::map;

void QWCompositorPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWCompositorPrivate::on_new_surface(void *data)
{
    auto *surface = QWSurface::from(reinterpret_cast<wlr_surface*>(data));
    Q_EMIT q_func()->newSurface(surface);
}

QWCompositor::QWCompositor(wlr_compositor *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWCompositorPrivate(handle, isOwner, this))
{

}

QWCompositor *QWCompositor::get(wlr_compositor *handle)
{
    return QWCompositorPrivate::map.value(handle);
}

QWCompositor *QWCompositor::from(wlr_compositor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWCompositor(handle, false);
}

QWCompositor *QWCompositor::create(QWDisplay *display, QWRenderer *renderer)
{
#if WLR_VERSION_MINOR > 16
    auto compositor = wlr_compositor_create(display->handle(), 5, renderer->handle());
#else
    auto compositor = wlr_compositor_create(display->handle(), renderer->handle());
#endif
    if (!compositor)
        return nullptr;
    return new QWCompositor(compositor, true);
}

/// QWSurface

class QWSurfacePrivate : public QWObjectPrivate
{
public:
    QWSurfacePrivate(wlr_surface *handle, bool isOwner, QWSurface *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSurfacePrivate::on_destroy);
        sc.connect(&handle->events.client_commit, this, &QWSurfacePrivate::on_client_commit);
        sc.connect(&handle->events.commit, this, &QWSurfacePrivate::on_commit);
        sc.connect(&handle->events.new_subsurface, this, &QWSurfacePrivate::on_new_subsurface);
#if WLR_VERSION_MINOR > 16
        sc.connect(&handle->events.precommit, this, &QWSurfacePrivate::on_precommit);
        sc.connect(&handle->events.map, this, &QWSurfacePrivate::on_map);
        sc.connect(&handle->events.unmap, this, &QWSurfacePrivate::on_unmap);
#endif
    }
    ~QWSurfacePrivate() {
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
    void on_client_commit(void *);
    void on_commit(void *);
    void on_new_subsurface(void *);
#if WLR_VERSION_MINOR > 16
    void on_precommit(void *);
    void on_map(void *);
    void on_unmap(void *);
#endif
    static QHash<void*, QWSurface*> map;
    QW_DECLARE_PUBLIC(QWSurface)
    QWSignalConnector sc;
};
QHash<void*, QWSurface*> QWSurfacePrivate::map;

void QWSurfacePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWSurfacePrivate::on_client_commit(void *)
{
    Q_EMIT q_func()->client_commit();
}

void QWSurfacePrivate::on_commit(void *)
{
    Q_EMIT q_func()->commit();
}

void QWSurfacePrivate::on_new_subsurface(void *)
{
    Q_EMIT q_func()->new_subsurface();
}

#if WLR_VERSION_MINOR > 16

void QWSurfacePrivate::on_precommit(void *data)
{
    Q_EMIT q_func()->precommit(reinterpret_cast<wlr_surface_state*>(data));
}

void QWSurfacePrivate::on_map(void *)
{
    Q_EMIT q_func()->map();
}

void QWSurfacePrivate::on_unmap(void *)
{
    Q_EMIT q_func()->unmap();
}

#endif

QWSurface::QWSurface(wlr_surface *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSurfacePrivate(handle, isOwner, this))
{

}

QWSurface *QWSurface::get(wlr_surface *handle)
{
    return QWSurfacePrivate::map.value(handle);
}

QWSurface *QWSurface::from(wlr_surface *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSurface(handle, false);
}

QWSurface *QWSurface::from(wl_resource *resource)
{
    auto *handle = wlr_surface_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

void QWSurface::destroyRoleObject()
{
    wlr_surface_destroy_role_object(handle());
}

void QWSurface::forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_surface_for_each_surface(handle(), iterator, userData);
}

QRectF QWSurface::getBufferSourceBox() const
{
    wlr_fbox wfbox = { 0 };
    wlr_surface_get_buffer_source_box(handle(), &wfbox);
    return QRectF { wfbox.x, wfbox.y, wfbox.width, wfbox.height };
}

void QWSurface::getEffectiveDamage(pixman_region32_t *damage) const
{
    wlr_surface_get_effective_damage(handle(), damage);
}

QRect QWSurface::getExtends() const
{
    wlr_box wbox = { 0 };
    wlr_surface_get_extends(handle(), &wbox);
    return QRect { wbox.x, wbox.y, wbox.width, wbox.height };
}

QWSurface *QWSurface::getRootSurface() const
{
    auto *surface = wlr_surface_get_root_surface(handle());
    if (!surface)
        return nullptr;
    return from(surface);
}

QWTexture *QWSurface::getTexture() const
{
    auto *texture = wlr_surface_get_texture(handle());
    if (!texture)
        return nullptr;
    return QWTexture::from(texture);
}

bool QWSurface::hasBuffer() const
{
    return wlr_surface_has_buffer(handle());
}

uint32_t QWSurface::lockPending()
{
    return wlr_surface_lock_pending(handle());
}

bool QWSurface::pointAcceptsInput(const QPointF& pos) const
{
    return wlr_surface_point_accepts_input(handle(), pos.x(), pos.y());
}

void QWSurface::sendEnter(QWOutput *output)
{
    wlr_surface_send_enter(handle(), output->handle());
}

void QWSurface::sendFrameDone(const timespec *when)
{
    wlr_surface_send_frame_done(handle(), when);
}

void QWSurface::sendLeave(QWOutput *output)
{
    wlr_surface_send_leave(handle(), output->handle());
}

QWSurface *QWSurface::surfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    auto *surface = wlr_surface_surface_at(handle(), xpos.x(), xpos.y(),
                                           subPos ? &subPos->rx() : nullptr,
                                           subPos ? &subPos->ry() : nullptr);
    if (!surface)
        return nullptr;
    return from(surface);
}

void QWSurface::unlockCached(uint32_t seq)
{
    wlr_surface_unlock_cached(handle(), seq);
}

#if WLR_VERSION_MINOR > 16
void QWSurface::setPreferredBufferScale(int32_t scale)
{
    wlr_surface_set_preferred_buffer_scale(handle(), scale);
}

void QWSurface::setPreferredBufferTransform(wl_output_transform_t transform)
{
   wlr_surface_set_preferred_buffer_transform(handle(), static_cast<wl_output_transform>(transform));
}

void QWSurface::setRole(const wlr_surface_role *role, wl_resource *errorResource, uint32_t errorCode)
{
   wlr_surface_set_role(handle(), role, errorResource, errorCode);
}
#endif

QW_END_NAMESPACE
