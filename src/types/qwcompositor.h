// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;
struct wlr_compositor;
struct wlr_surface;
struct wlr_surface_state;
struct wl_resource;
struct wlr_surface_role;
struct pixman_region32;
typedef pixman_region32 pixman_region32_t;
typedef uint32_t wl_output_transform_t;

using wlr_surface_iterator_func_t = void (*)(wlr_surface *surface, int sx, int sy, void *data);

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWRenderer;
class QWTexture;
class QWOutput;
class QWSurface;
class QWCompositorPrivate;
class QW_EXPORT QWCompositor : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWCompositor)
public:
    inline wlr_compositor *handle() const {
        return QWObject::handle<wlr_compositor>();
    }

    static QWCompositor *get(wlr_compositor *handle);
    static QWCompositor *from(wlr_compositor *handle);
    static QWCompositor *create(QWDisplay *display, QWRenderer *renderer);

Q_SIGNALS:
    void beforeDestroy(QWCompositor *self);
    void newSurface(QWSurface *surface);

private:
    QWCompositor(wlr_compositor *handle, bool isOwner);
    ~QWCompositor() = default;
};

class QWSurfacePrivate;
class QW_EXPORT QWSurface : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSurface)
public:
    inline wlr_surface *handle() const {
        return QWObject::handle<wlr_surface>();
    }

    static QWSurface *get(wlr_surface *handle);
    static QWSurface *from(wlr_surface *handle);
    static QWSurface *from(wl_resource *resource);

    void destroyRoleObject();

    void forEachSurface(wlr_surface_iterator_func_t iterator, void *userData) const;
    QRectF getBufferSourceBox() const;
    void getEffectiveDamage(pixman_region32_t *damage) const;
    QRect getExtends() const;
    QWSurface *getRootSurface() const;
    QWTexture *getTexture() const;
    bool hasBuffer() const;
    uint32_t lockPending();
    bool pointAcceptsInput(const QPointF& pos) const;
    void sendEnter(QWOutput *output);
    void sendFrameDone(const timespec *when);
    void sendLeave(QWOutput *output);
    QWSurface *surfaceAt(const QPointF &xpos, QPointF *subPos = nullptr) const;
    void unlockCached(uint32_t seq);
#if WLR_VERSION_MINOR > 16
    void setPreferredBufferScale(int32_t scale);
    void setPreferredBufferTransform(wl_output_transform_t transform);
    void setRole(const wlr_surface_role *role, wl_resource *errorResource, uint32_t errorCode);
#endif

Q_SIGNALS:
    void beforeDestroy(QWSurface *self);
    void client_commit();
    void commit();
    void new_subsurface();
    void map();
    void unmap();
#if WLR_VERSION_MINOR > 16
    void precommit(const wlr_surface_state *state);
#endif

private:
    QWSurface(wlr_surface *handle, bool isOwner);
    ~QWSurface() = default;
};


QW_END_NAMESPACE
