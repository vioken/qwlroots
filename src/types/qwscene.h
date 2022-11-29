// Copyright (C) 2022 zccrs zccrs@live.com.
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
struct wlr_surface;
struct wlr_scene_rect;
struct wlr_scene_output;
struct wlr_buffer;
struct wlr_fbox;
struct wlr_output;
struct wlr_xdg_surface;

struct pixman_region32;

typedef bool (*wlr_scene_buffer_point_accepts_input_func_t)(
    struct wlr_scene_buffer *buffer, int sx, int sy);

typedef void (*wlr_scene_buffer_iterator_func_t)(
    struct wlr_scene_buffer *buffer, int sx, int sy, void *user_data);

QT_BEGIN_NAMESPACE
class QPoint;
class QPointF;
class QSize;
class QColor;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QWXdgSurface;
class QWSceneNodePrivate;
class QW_EXPORT QWSceneNode : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWSceneNode)
public:
    explicit QWSceneNode(wlr_scene_node *handle);

    inline wlr_scene_node *handle() const {
        return QWObject::handle<wlr_scene_node>();
    }

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

protected:
    QWSceneNode(QWSceneNodePrivate &dd);
};

class QW_EXPORT QWSceneTree : public QWSceneNode
{
public:
    explicit QWSceneTree(wlr_scene_tree *handle);

    inline wlr_scene_tree *handle() const {
        return fromNode(QWSceneNode::handle());
    }

    static wlr_scene_tree *fromNode(wlr_scene_node *node);
    static QWSceneTree *create(QWSceneTree *parent);
    static QWSceneTree *subsurfaceTreeCreate(QWSceneTree *parent, wlr_surface *surface);
    static QWSceneTree *xdgSurfaceCreate(QWSceneTree *parent, QWXdgSurface *xdgSurface);

protected:
    QWSceneTree(QWSceneNodePrivate &dd);
};

class QWOutputLayout;
class QW_EXPORT QWScene : public QWSceneTree
{
public:
    explicit QWScene(wlr_scene *handle);

    wlr_scene *handle() const;

    static QWScene *create();

    void setPresentation(wlr_presentation *presentation);
    bool attachOutputLayout(QWOutputLayout *outputLayout);
};

class QWBuffer;
class QWSceneBufferPrivate;
class QW_EXPORT QWSceneBuffer : public QWSceneNode
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSceneBuffer)
public:
    explicit QWSceneBuffer(wlr_scene_buffer *handle);

    inline wlr_scene_buffer *handle() const {
        return fromNode(QWSceneNode::handle());
    }

    static wlr_scene_buffer *fromNode(wlr_scene_node *node);
    static QWSceneBuffer *create(QWSceneTree *parent, wlr_buffer *buffer);

    void setBuffer(QWBuffer *buffer);
    void setBuffer(QWBuffer *buffer, pixman_region32 *region);
    void setOpaqueRegion(pixman_region32 *region);
    void setSourceBox(const QRectF &box);
    void setDestSize(const QSize &size);
    void setTransform(int wl_output_transform);
    void sendFrameDone(timespec *now);

Q_SIGNALS:
    void outputEnter(wlr_scene_output *output);
    void outputLeave(wlr_scene_output *output);
    void outputPresent(wlr_scene_output *output);
    void frameDone(timespec *now);
};

class QW_EXPORT QWSceneRect : public QWSceneNode
{
public:
    explicit QWSceneRect(wlr_scene_rect *handle);

    inline wlr_scene_rect *handle() const {
        return QWObject::handle<wlr_scene_rect>();
    }

    static wlr_scene_rect *fromeNode(wlr_scene_node *node);
    static QWSceneRect *create(QWSceneTree *parent, const QSize &size, const QColor &color);

    void setSize(const QSize &size);
    void setColor(const QColor &color);
};

class QWOutput;
class QWSceneOutputPrivate;
class QW_EXPORT QWSceneOutput : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWSceneOutput)
public:
    explicit QWSceneOutput(wlr_scene_output *handle);

    inline wlr_scene_output *handle() const {
        return QWObject::handle<wlr_scene_output>();
    }

    static QWSceneOutput *create(QWScene *scene, QWOutput *output);
    static wlr_scene_output *get(QWScene *scene, QWOutput *output);

    void commit();
    void sendFrameDone(timespec *now);

    void forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *user_data) const;
};

QW_END_NAMESPACE
