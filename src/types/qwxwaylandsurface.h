// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR
// GPL-3.0-only

#pragma once

#include <qwglobal.h>

typedef uint32_t wlr_xwayland_icccm_input_model_t;
typedef uint32_t xcb_stack_mode;

struct wlr_xwayland_surface;
class QRect;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWXWaylandSurface {
public:
    QWXWaylandSurface() = delete;
    ~QWXWaylandSurface() = delete;

    static QWXWaylandSurface* from(wlr_xwayland_surface* surface);
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
};

QW_END_NAMESPACE
