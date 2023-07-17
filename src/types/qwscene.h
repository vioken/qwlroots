// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_scene;
struct wlr_scene_node;
struct wlr_scene_tree;
struct wlr_presentation;
struct wlr_scene_surface;
struct wlr_scene_buffer;
struct wlr_scene_rect;
struct wlr_scene_output;
struct wlr_buffer;
struct wlr_fbox;
struct wlr_output;
struct wlr_xdg_surface;
struct wlr_scene_output_state_options;
struct wlr_scene_output_sample_event;

struct pixman_region32;

typedef uint32_t wl_output_transform_t;

using wlr_scene_buffer_point_accepts_input_func_t = bool (*)(wlr_scene_buffer *buffer, int sx, int sy);

using wlr_scene_buffer_iterator_func_t = void (*)(wlr_scene_buffer *buffer, int sx, int sy, void *user_data);

QT_BEGIN_NAMESPACE
class QPoint;
class QPointF;
class QSize;
class QColor;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QWSurface;
class QWXdgSurface;
class QWSceneNodePrivate;
class QW_EXPORT QWSceneNode : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSceneNode)
public:
    virtual ~QWSceneNode() override = default;

    inline wlr_scene_node *handle() const {
        return QWObject::handle<wlr_scene_node>();
    }

    static QWSceneNode *get(wlr_scene_node *handle);

    void setEnabled(bool enabled);
    void setPosition(const QPoint &pos);
    void placeAbove(wlr_scene_node *sibling);
    void placeBelow(wlr_scene_node *sibling);
    void raiseToTop();
    void lowerToBottom();
    void reparent(wlr_scene_tree *newParent);

    bool coords(QPoint &pos) const;
    void forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *userData) const;
    wlr_scene_node *at(const QPointF &lpos, QPointF *npos = nullptr) const;

Q_SIGNALS:
    void beforeDestroy(QWSceneNode *self);

protected:
    QWSceneNode(QWSceneNodePrivate &dd);
    QWSceneNode(wlr_scene_node *handle, bool isOwner);
};

class QW_EXPORT QWSceneTree : public QWSceneNode
{
    Q_OBJECT
public:
    ~QWSceneTree() override = default;
    explicit QWSceneTree(QWSceneTree *parent);

    wlr_scene_tree *handle() const;

    static QWSceneTree *get(wlr_scene_tree *handle);
    static QWSceneTree *from(wlr_scene_tree *handle);
    static QWSceneTree *from(wlr_scene_node *node);

    static QWSceneTree *subsurfaceTreeCreate(QWSceneTree *parent, QWSurface *surface);
    static QWSceneTree *xdgSurfaceCreate(QWSceneTree *parent, QWXdgSurface *xdgSurface);

protected:
    QWSceneTree(QWSceneNodePrivate &dd);
    QWSceneTree(wlr_scene_tree *handle, bool isOwner);
};

class QWOutputLayout;
class QW_EXPORT QWScene : public QWSceneTree
{
    Q_OBJECT
public:
    ~QWScene() override = default;
    explicit QWScene(QObject *parent = nullptr);

    wlr_scene *handle() const;

    static QWScene *get(wlr_scene *handle);
    static QWScene *from(wlr_scene *handle);

    void setPresentation(wlr_presentation *presentation);
    bool attachOutputLayout(QWOutputLayout *outputLayout);

private:
    QWScene(wlr_scene *handle, bool isOwner, QObject *parent);
};

class QWBuffer;
class QWSceneBufferPrivate;
class QW_EXPORT QWSceneBuffer : public QWSceneNode
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSceneBuffer)
public:
    ~QWSceneBuffer() override = default;
    explicit QWSceneBuffer(QWBuffer *buffer, QWSceneTree *parent);

    wlr_scene_buffer *handle() const;

    static QWSceneBuffer *get(wlr_scene_buffer *handle);
    static QWSceneBuffer *from(wlr_scene_buffer *handle);
    static QWSceneBuffer *from(wlr_scene_node *node);

    void setBuffer(QWBuffer *buffer);
    void setBuffer(QWBuffer *buffer, pixman_region32 *region);
    void setOpaqueRegion(pixman_region32 *region);
    void setSourceBox(const QRectF &box);
    void setDestSize(const QSize &size);
    void setTransform(wl_output_transform_t wl_output_transform);
    void sendFrameDone(timespec *now);

Q_SIGNALS:
    void outputEnter(wlr_scene_output *output);
    void outputLeave(wlr_scene_output *output);
#if WLR_VERSION_MINOR > 16
    void outputSample(wlr_scene_output_sample_event *output);
#else
    void outputPresent(wlr_scene_output *output);
#endif
    void frameDone(timespec *now);

private:
    QWSceneBuffer(wlr_scene_buffer *handle, bool isOwner);
};

class QW_EXPORT QWSceneRect : public QWSceneNode
{
    Q_OBJECT
public:
    ~QWSceneRect() override = default;
    explicit QWSceneRect(const QSize &size, const QColor &color, QWSceneTree *parent);

    inline wlr_scene_rect *handle() const {
        return QWObject::handle<wlr_scene_rect>();
    }

    static QWSceneRect *get(wlr_scene_rect *handle);
    static QWSceneRect *from(wlr_scene_rect *handle);
    static QWSceneRect *frome(wlr_scene_node *node);

    void setSize(const QSize &size);
    void setColor(const QColor &color);

private:
    QWSceneRect(wlr_scene_rect *handle, bool isOwner);
};

class QWOutput;
class QWSceneOutputPrivate;
class QW_EXPORT QWSceneOutput : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSceneOutput)
public:
    ~QWSceneOutput() override = default;
    explicit QWSceneOutput(QWScene *scene, QWOutput *output);

    inline wlr_scene_output *handle() const {
        return QWObject::handle<wlr_scene_output>();
    }

    static QWSceneOutput *get(wlr_scene_output *handle);
    static QWSceneOutput *from(wlr_scene_output *handle);
    static QWSceneOutput *from(QWScene *scene, QWOutput *output);
#if WLR_VERSION_MINOR > 16
    void commit(const wlr_scene_output_state_options *options);
#else
    void commit();
#endif
    void sendFrameDone(timespec *now);

    void forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *user_data) const;

Q_SIGNALS:
    void beforeDestroy(QWSceneOutput *self);

private:
    QWSceneOutput(wlr_scene_output *handle, bool isOwner);
};

QW_END_NAMESPACE
