// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlayershellv1.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "qwxdgshell.h"
#include "private/qwglobal_p.h"

#include <QHash>
#include <QPointF>

extern "C" {
// avoid replace namespace
#include <math.h>
#define namespace scope
#define static
#include <wlr/types/wlr_layer_shell_v1.h>
#undef namespace
#undef static
}

QW_BEGIN_NAMESPACE

/// QWLayerShellV1

class QWLayerShellV1Private : public QWWrapObjectPrivate
{
public:
    QWLayerShellV1Private(wlr_layer_shell_v1 *handle, bool isOwner, QWLayerShellV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_surface, this, &QWLayerShellV1Private::on_new_surface);
    }

    void on_new_surface(void *);

    QW_DECLARE_PUBLIC(QWLayerShellV1)
};

void QWLayerShellV1Private::on_new_surface(void *data)
{
    auto *surface = QWLayerSurfaceV1::from(reinterpret_cast<wlr_layer_surface_v1*>(data));
    Q_EMIT q_func()->newSurface(surface);
}

QWLayerShellV1::QWLayerShellV1(wlr_layer_shell_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWLayerShellV1Private(handle, isOwner, this))
{

}

QWLayerShellV1 *QWLayerShellV1::create(QWDisplay *display, uint32_t version)
{
    auto handle = wlr_layer_shell_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWLayerShellV1(handle, true);
}

QWLayerShellV1 *QWLayerShellV1::get(wlr_layer_shell_v1 *handle)
{
    return static_cast<QWLayerShellV1*>(QWLayerShellV1Private::map.value(handle));
}

QWLayerShellV1 *QWLayerShellV1::from(wlr_layer_shell_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWLayerShellV1(handle, false);
}

/// QWLayerSurfaceV1

class QWLayerSurfaceV1Private : public QWWrapObjectPrivate
{
public:
    QWLayerSurfaceV1Private(wlr_layer_surface_v1 *handle, bool isOwner, QWLayerSurfaceV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_layer_surface_v1_destroy))
    {
        sc.connect(&handle->events.new_popup, this, &QWLayerSurfaceV1Private::on_new_popup);
    }

    void on_new_popup(void *);

    QW_DECLARE_PUBLIC(QWLayerSurfaceV1)
};

void QWLayerSurfaceV1Private::on_new_popup(void *data)
{
    auto *popup = QWXdgPopup::from(static_cast<wlr_xdg_popup*>(data));
    Q_EMIT q_func()->newPopup(popup);
}

QWLayerSurfaceV1::QWLayerSurfaceV1(wlr_layer_surface_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWLayerSurfaceV1Private(handle, isOwner, this))
{


}

QWLayerSurfaceV1 *QWLayerSurfaceV1::get(wlr_layer_surface_v1 *handle)
{
    return static_cast<QWLayerSurfaceV1*>(QWLayerSurfaceV1Private::map.value(handle));
}

QWLayerSurfaceV1 *QWLayerSurfaceV1::from(wlr_layer_surface_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWLayerSurfaceV1(handle, false);
}

QWLayerSurfaceV1 *QWLayerSurfaceV1::from(wl_resource *resource)
{
    auto *handle = wlr_layer_surface_v1_from_resource(resource);
    if (!handle)
        return nullptr;
    return from(handle);
}

QWLayerSurfaceV1 *QWLayerSurfaceV1::from(wlr_surface *surface)
{
    auto *handle = wlr_layer_surface_v1_try_from_wlr_surface(surface);
    if (!handle)
        return nullptr;
    return from(handle);
}

QWLayerSurfaceV1 *QWLayerSurfaceV1::from(QWSurface *surface)
{
    return from(surface->handle());
}

uint32_t QWLayerSurfaceV1::configure(uint32_t width, uint32_t height)
{
    return wlr_layer_surface_v1_configure(handle(), width, height);
}

void QWLayerSurfaceV1::forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_layer_surface_v1_for_each_surface(handle(), iterator, userData);
}

void QWLayerSurfaceV1::forEachPopupSurface(wlr_surface_iterator_func_t iterator, void *userData) const
{
    wlr_layer_surface_v1_for_each_popup_surface(handle(), iterator, userData);
}

QWSurface *QWLayerSurfaceV1::surfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    auto* surface = wlr_layer_surface_v1_surface_at(handle(), xpos.x(), xpos.y(),
                                           subPos ? &subPos->rx() : nullptr,
                                           subPos ? &subPos->ry() : nullptr);
    if (!surface)
        return nullptr;
    return QWSurface::from(surface);
}

QWSurface *QWLayerSurfaceV1::popupSurfaceAt(const QPointF &xpos, QPointF *subPos) const
{
    auto* surface = wlr_layer_surface_v1_popup_surface_at(handle(), xpos.x(), xpos.y(),
                                           subPos ? &subPos->rx() : nullptr,
                                           subPos ? &subPos->ry() : nullptr);
    if (!surface)
        return nullptr;
    return QWSurface::from(surface);
}

QWSurface *QWLayerSurfaceV1::surface() const
{
    return QWSurface::from(handle()->surface);
}

QW_END_NAMESPACE
