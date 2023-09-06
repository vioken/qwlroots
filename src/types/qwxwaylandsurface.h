// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR
// GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

typedef uint32_t wlr_xwayland_icccm_input_model_t;
typedef uint32_t xcb_stack_mode;

struct wlr_xwayland_surface;
struct wlr_xwayland_surface_configure_event;
struct wlr_xwayland_resize_event;
struct wlr_xwayland_minimize_event;

QT_BEGIN_NAMESPACE
class QRect;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QWSurface;
class QWXWaylandSurfacePrivate;
class QW_EXPORT QWXWaylandSurface : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXWaylandSurface)
public:
    static QWXWaylandSurface *get(wlr_xwayland_surface* handle);
    static QWXWaylandSurface* from(wlr_xwayland_surface* handle);
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
    static QWXWaylandSurface *tryFromWlrSurface(QWSurface *surface);
#endif

    wlr_xwayland_surface* handle() const;
    void activate(bool activated);
    void restack(QWXWaylandSurface *sibling, xcb_stack_mode mode);
    void configure(const QRect &rect);
    void close();
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
    void setWithdrawn(bool withdrawn);
#endif
    void setMinimized(bool minimized);
    void setMaximized(bool maximized);
    void setFullscreen(bool fullscreen);
    void ping();
    bool xwaylandOrSurfaceWantsFocus() const;
    wlr_xwayland_icccm_input_model_t icccmInputModel() const;

Q_SIGNALS:
    void beforeDestroy(QWXWaylandSurface *self);
    void requestConfigure(wlr_xwayland_surface_configure_event *event);
    void requestMove();
    void requestResize(wlr_xwayland_resize_event *event);
    void requestMinimize(wlr_xwayland_minimize_event *event);
    void requestMaximize();
    void requestFullscreen();
    void requestActivate();
    void titleChanged();
    void classChanged();
    void roleChanged();
    void parentChanged();
    void startupIdChanged();
    void windowTypeChanged();
    void hintsChanged();
    void decorationsChanged();
    void overrideRedirectChanged();
    void geometryChanged();
    void pingTimeout();
#if WLR_VERSION_MINOR > 16
    void associate();
    void dissociate();
    void strutPartialChanged();
#endif

private:
    QWXWaylandSurface(wlr_xwayland_surface *handle);
    ~QWXWaylandSurface() override = default;
};

QW_END_NAMESPACE
