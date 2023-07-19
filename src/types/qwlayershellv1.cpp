// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlayershellv1.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "qwxdgshell.h"
#include "util/qwsignalconnector.h"

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

class QWLayerShellV1Private : public QWObjectPrivate
{
public:
    QWLayerShellV1Private(wlr_layer_shell_v1 *handle, bool isOwner, QWLayerShellV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_surface, this, &QWLayerShellV1Private::on_new_surface);
        sc.connect(&handle->events.destroy, this, &QWLayerShellV1Private::on_destroy);
    }
    ~QWLayerShellV1Private() {
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

    void on_new_surface(void *);
    void on_destroy(void *);

    static QHash<void*, QWLayerShellV1*> map;
    QW_DECLARE_PUBLIC(QWLayerShellV1)
    QWSignalConnector sc;
};
QHash<void*, QWLayerShellV1*> QWLayerShellV1Private::map;

void QWLayerShellV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWLayerShellV1Private::on_new_surface(void *data)
{
    Q_EMIT q_func()->newSurface(reinterpret_cast<wlr_layer_surface_v1*>(data));
}

QWLayerShellV1::QWLayerShellV1(wlr_layer_shell_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWLayerShellV1Private(handle, isOwner, this))
{

}

#if WLR_VERSION_MINOR > 16
QWLayerShellV1 *QWLayerShellV1::create(QWDisplay *display, uint32_t version)
{
    auto handle = wlr_layer_shell_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWLayerShellV1(handle, true);
}
#else
QWLayerShellV1 *QWLayerShellV1::create(QWDisplay *display)
{
    auto handle = wlr_layer_shell_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWLayerShellV1(handle, true);
}
#endif // WLR_VERSION_MINOR > 16

QWLayerShellV1 *QWLayerShellV1::get(wlr_layer_shell_v1 *handle)
{
    return QWLayerShellV1Private::map.value(handle);
}

QWLayerShellV1 *QWLayerShellV1::from(wlr_layer_shell_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWLayerShellV1(handle, false);
}

/// QWLayerSurfaceV1

class QWLayerSurfaceV1Private : public QWObjectPrivate
{
public:
    QWLayerSurfaceV1Private(wlr_layer_surface_v1 *handle, bool isOwner, QWLayerSurfaceV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
#if WLR_VERSION_MINOR <= 16
        sc.connect(&handle->events.map, this, &QWLayerSurfaceV1Private::on_map);
        sc.connect(&handle->events.unmap, this, &QWLayerSurfaceV1Private::on_unmap);
#endif
        sc.connect(&handle->events.new_popup, this, &QWLayerSurfaceV1Private::on_new_popup);
        sc.connect(&handle->events.destroy, this, &QWLayerSurfaceV1Private::on_destroy);
    }
    ~QWLayerSurfaceV1Private() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_layer_surface_v1_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

#if WLR_VERSION_MINOR <= 16
    void on_map(void *);
    void on_unmap(void *);
#endif
    void on_new_popup(void *);
    void on_destroy(void *);

    static QHash<void*, QWLayerSurfaceV1*> map;
    QW_DECLARE_PUBLIC(QWLayerSurfaceV1)
    QWSignalConnector sc;
};
QHash<void*, QWLayerSurfaceV1*> QWLayerSurfaceV1Private::map;

void QWLayerSurfaceV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

#if WLR_VERSION_MINOR <= 16
void QWLayerSurfaceV1Private::on_map(void *)
{
    Q_EMIT q_func()->surface()->map();
}

void QWLayerSurfaceV1Private::on_unmap(void *)
{
    Q_EMIT q_func()->surface()->unmap();
}
#endif

void QWLayerSurfaceV1Private::on_new_popup(void *data)
{
    auto *popup = QWXdgPopup::from(static_cast<wlr_xdg_popup*>(data));
    Q_EMIT q_func()->newPopup(popup);
}

QWLayerSurfaceV1::QWLayerSurfaceV1(wlr_layer_surface_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWLayerSurfaceV1Private(handle, isOwner, this))
{


}

QWLayerSurfaceV1 *QWLayerSurfaceV1::get(wlr_layer_surface_v1 *handle)
{
    return QWLayerSurfaceV1Private::map.value(handle);
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

QWLayerSurfaceV1 *QWLayerSurfaceV1::from(QWSurface *surface)
{
#if WLR_VERSION_MINOR > 16
    auto *handle = wlr_layer_surface_v1_try_from_wlr_surface(surface->handle());
#else
    if (!wlr_surface_is_layer_surface(surface->handle()))
        return nullptr;
    auto *handle = wlr_layer_surface_v1_from_wlr_surface(surface->handle());
#endif
    if (!handle)
        return nullptr;
    return from(handle);
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
