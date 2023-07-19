// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwconfig.h>
#include <QObject>

struct wlr_layer_surface_v1;
struct wlr_layer_shell_v1;
struct wl_resource;
struct wlr_surface;

using wlr_surface_iterator_func_t = void (*)(wlr_surface *surface, int sx, int sy, void *data);

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSurface;
class QWLayerShellV1Private;
class QWXdgPopup;
class QW_EXPORT QWLayerShellV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWLayerShellV1)
public:
#if WLR_VERSION_MINOR > 16
    static QWLayerShellV1 *create(QWDisplay *display, uint32_t version);
#else
    static QWLayerShellV1 *create(QWDisplay *display);
#endif
    inline wlr_layer_shell_v1 *handle() const {
        return QWObject::handle<wlr_layer_shell_v1>();
    }

    static QWLayerShellV1 *get(wlr_layer_shell_v1 *handle);
    static QWLayerShellV1 *from(wlr_layer_shell_v1 *handle);

Q_SIGNALS:
    void newSurface(wlr_layer_surface_v1 *surface);
    void beforeDestroy(QWLayerShellV1 *self);

private:
    QWLayerShellV1(wlr_layer_shell_v1 *handle, bool isOwner);
    ~QWLayerShellV1() = default;
};

class QWLayerSurfaceV1Private;
class QW_EXPORT QWLayerSurfaceV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWLayerSurfaceV1)
public:
    static QWLayerShellV1 *create(QWDisplay *display);
    inline wlr_layer_surface_v1 *handle() const {
        return QWObject::handle<wlr_layer_surface_v1>();
    }

    static QWLayerSurfaceV1 *get(wlr_layer_surface_v1 *handle);
    static QWLayerSurfaceV1 *from(wlr_layer_surface_v1 *handle);
    static QWLayerSurfaceV1 *from(wl_resource *resource);
    static QWLayerSurfaceV1 *from(QWSurface *surface);

    uint32_t configure(uint32_t width, uint32_t height);
    void forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const;
    QWSurface *surfaceAt(const QPointF &xpos, QPointF *subPos = nullptr) const;
    QWSurface *popupSurfaceAt(const QPointF &xpos, QPointF *subPos = nullptr) const;
    QRect getGeometry() const;
    void forEachPopupSurface(wlr_surface_iterator_func_t iterator, void *userData) const;
    QWSurface *surface() const;

Q_SIGNALS:
    void beforeDestroy(QWLayerSurfaceV1 *self);
    void newPopup(QWXdgPopup *popup);

private:
    QWLayerSurfaceV1(wlr_layer_surface_v1 *handle, bool isOwner);
    ~QWLayerSurfaceV1() = default;
};

QW_END_NAMESPACE
