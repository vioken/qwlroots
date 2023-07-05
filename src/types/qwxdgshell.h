// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_xdg_shell;
struct wlr_xdg_surface;
struct wlr_xdg_popup;
struct wlr_xdg_toplevel;
struct wlr_xdg_toplevel_move_event;
struct wlr_xdg_toplevel_resize_event;
struct wlr_xdg_toplevel_show_window_menu_event;
struct wlr_xdg_surface_configure;
struct wlr_surface;
struct wl_resource;

using wlr_surface_iterator_func_t = void (*)(wlr_surface *surface, int sx, int sy, void *data);

QW_BEGIN_NAMESPACE

class QWSurface;
class QWDisplay;
class QWXdgShellPrivate;
class QW_EXPORT QWXdgShell : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgShell)
public:
    static QWXdgShell *create(QWDisplay *display, uint32_t version);

    inline wlr_xdg_shell *handle() const {
        return QWObject::handle<wlr_xdg_shell>();
    }

    static QWXdgShell *get(wlr_xdg_shell *handle);
    static QWXdgShell *from(wlr_xdg_shell *handle);

Q_SIGNALS:
    void beforeDestroy(QWXdgShell *self);
    void newSurface(wlr_xdg_surface *surface);

private:
    QWXdgShell(wlr_xdg_shell *handle, bool isOwner);
    ~QWXdgShell() = default;
};

class QWXdgPopup;
class QWXdgToplevel;
class QWXdgSurfacePrivate;
class QW_EXPORT QWXdgSurface : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgSurface)
public:
    inline wlr_xdg_surface *handle() const {
        return QWObject::handle<wlr_xdg_surface>();
    }

    static QWXdgSurface *get(wlr_xdg_surface *handle);
    static QWXdgSurface *from(wl_resource *resource);
    static QWXdgSurface *from(QWSurface *surface);

    QWXdgPopup *toPopup() const;
    QWXdgToplevel *topToplevel() const;
    QWSurface *surface() const;

    void ping();
    uint32_t scheduleConfigure();

    QWSurface *surfaceAt(const QPointF &xpos, QPointF *subPos = nullptr) const;
    QWSurface *popupSurfaceAt(const QPointF &xpos, QPointF *subPos = nullptr) const;
    QRect getGeometry() const;
    void forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const;
    void forEachPopupSurface(wlr_surface_iterator_func_t iterator, void *userData) const;

Q_SIGNALS:
    void beforeDestroy(QWXdgSurface *self);
    void pingTimeout();
    void newPopup(QWXdgPopup *popup);
    void configure(wlr_xdg_surface_configure *conf);
    void ackConfigure(wlr_xdg_surface_configure *conf);

protected:
    QWXdgSurface(QWXdgSurfacePrivate &dd);
    QWXdgSurface(wlr_xdg_surface *handle, bool isOwner);
    ~QWXdgSurface() = default;
};

class QWXdgPopupPrivate;
class QW_EXPORT QWXdgPopup : public QWXdgSurface
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgPopup)
public:
    ~QWXdgPopup() = default;

    wlr_xdg_popup *handle() const;

    static QWXdgPopup *get(wlr_xdg_popup *handle);
    static QWXdgPopup *from(wlr_xdg_popup *handle);
    static QWXdgPopup *from(wl_resource *resource);

    QPointF getPosition() const;
    QPoint getToplevelCoords(const QPoint &popupsPos) const;

    void unconstrainFromBox(const QRect &toplevelSpaceBox);

Q_SIGNALS:
    void reposition();

private:
    QWXdgPopup(wlr_xdg_popup *handle, bool isOwner);
};

class QWXdgToplevelPrivate;
class QW_EXPORT QWXdgToplevel : public QWXdgSurface
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgToplevel)
public:
    wlr_xdg_toplevel *handle() const;

    static QWXdgToplevel *get(wlr_xdg_toplevel *handle);
    static QWXdgToplevel *from(wlr_xdg_toplevel *handle);
    static QWXdgToplevel *from(wl_resource *resource);

    uint32_t setSize(const QSize &size);
    uint32_t setActivated(bool activated);
    uint32_t setMaximized(bool maximized);
    uint32_t setFullscreen(bool fullscreen);
    uint32_t setResizing(bool resizing);
    uint32_t setTiled(uint32_t tiledEdges);
    uint32_t setBounds(const QSize &bounds);
    uint32_t setWmCapabilities(uint32_t caps);

    void sendClose();
    bool setParent(QWXdgToplevel *parent);

Q_SIGNALS:
    void requestMaximize(bool maximize);
    void requestFullscreen(bool fullscreen);
    void requestMinimize(bool minimize);
    void requestMove(wlr_xdg_toplevel_move_event *event);
    void requestResize(wlr_xdg_toplevel_resize_event *event);
    void requestShowWindowMenu(wlr_xdg_toplevel_show_window_menu_event *event);

    void parentChanged(QWXdgToplevel *newParent);
    void titleChanged(char *newTitle);
    void appidChanged(char *newAppid);

private:
    QWXdgToplevel(wlr_xdg_toplevel *handle, bool isOwner);
    ~QWXdgToplevel() = default;
};

QW_END_NAMESPACE
