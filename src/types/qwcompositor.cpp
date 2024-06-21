// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcompositor.h"
#include "qwsubcompositor.h"
#include "qwdisplay.h"
#include "qw_texture.h"
#include "qwoutput.h"
#include "render/qwrenderer.h"
#include "private/qwglobal_p.h"

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

class QWCompositorPrivate : public QWWrapObjectPrivate
{
public:
    QWCompositorPrivate(wlr_compositor *handle, bool isOwner, QWCompositor *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_surface, this, &QWCompositorPrivate::on_new_surface);
    }

    void on_new_surface(void *data);

    QW_DECLARE_PUBLIC(QWCompositor)
};

void QWCompositorPrivate::on_new_surface(void *data)
{
    auto *surface = QWSurface::from(reinterpret_cast<wlr_surface*>(data));
    Q_EMIT q_func()->newSurface(surface);
}

QWCompositor::QWCompositor(wlr_compositor *handle, bool isOwner)
    : QWWrapObject(*new QWCompositorPrivate(handle, isOwner, this))
{

}

QWCompositor *QWCompositor::get(wlr_compositor *handle)
{
    return static_cast<QWCompositor*>(QWCompositorPrivate::map.value(handle));
}

QWCompositor *QWCompositor::from(wlr_compositor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWCompositor(handle, false);
}

QWCompositor *QWCompositor::create(QWDisplay *display, QWRenderer *renderer, uint32_t version)
{
    auto compositor = wlr_compositor_create(display->handle(), version, renderer->handle());
    if (!compositor)
        return nullptr;
    return new QWCompositor(compositor, true);
}

/// QWSurface

class QWSurfacePrivate : public QWWrapObjectPrivate
{
public:
    QWSurfacePrivate(wlr_surface *handle, bool isOwner, QWSurface *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.client_commit, this, &QWSurfacePrivate::on_client_commit);
        sc.connect(&handle->events.commit, this, &QWSurfacePrivate::on_commit);
        sc.connect(&handle->events.new_subsurface, this, &QWSurfacePrivate::on_new_subsurface);
#if WLR_VERSION_MINOR < 18
        sc.connect(&handle->events.precommit, this, &QWSurfacePrivate::on_precommit);
#endif
        sc.connect(&handle->events.map, this, &QWSurfacePrivate::on_map);
        sc.connect(&handle->events.unmap, this, &QWSurfacePrivate::on_unmap);
    }

    void on_client_commit(void *);
    void on_commit(void *);
    void on_new_subsurface(void *);
    void on_precommit(void *);
    void on_map(void *);
    void on_unmap(void *);

    QW_DECLARE_PUBLIC(QWSurface)
};

void QWSurfacePrivate::on_client_commit(void *)
{
    Q_EMIT q_func()->clientCommit();
}

void QWSurfacePrivate::on_commit(void *)
{
    Q_EMIT q_func()->commit();
}

void QWSurfacePrivate::on_new_subsurface(void *data)
{
    auto handle = reinterpret_cast<wlr_subsurface*>(data);
    Q_ASSERT(handle);
    Q_EMIT q_func()->newSubsurface(QWSubsurface::from(handle));
}

void QWSurfacePrivate::on_precommit(void *data)
{
    Q_EMIT q_func()->precommit(reinterpret_cast<wlr_surface_state*>(data));
}

void QWSurfacePrivate::on_map(void *)
{
    Q_EMIT q_func()->mapped();
}

void QWSurfacePrivate::on_unmap(void *)
{
    Q_EMIT q_func()->unmapped();
}

QWSurface::QWSurface(wlr_surface *handle, bool isOwner)
    : QWWrapObject(*new QWSurfacePrivate(handle, isOwner, this))
{

}

QWSurface *QWSurface::get(wlr_surface *handle)
{
    return static_cast<QWSurface*>(QWSurfacePrivate::map.value(handle));
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

void QWSurface::forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_surface_for_each_surface(handle(), iterator, userData);
}

QRectF QWSurface::getBufferSourceBox() const
{
    wlr_fbox wfbox = { 0, 0, 0, 0 };
    wlr_surface_get_buffer_source_box(handle(), &wfbox);
    return QRectF { wfbox.x, wfbox.y, wfbox.width, wfbox.height };
}

void QWSurface::getEffectiveDamage(pixman_region32_t *damage) const
{
    wlr_surface_get_effective_damage(handle(), damage);
}

QRect QWSurface::getExtends() const
{
    wlr_box wbox = { 0, 0, 0, 0 };
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

void QWSurface::map()
{
    wlr_surface_map(handle());
}

void QWSurface::unmap()
{
    wlr_surface_unmap(handle());
}

QW_END_NAMESPACE
