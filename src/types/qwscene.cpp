// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwscene.h"
#include "qwoutputlayout.h"
#include "qwbuffer.h"
#include "util/qwsignalconnector.h"

#include <QColor>
#include <QPoint>
#include <QRect>

extern "C" {
#define static
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWSceneNodePrivate : public QWObjectPrivate
{
public:
    QWSceneNodePrivate(void *handle, QWSceneNode *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.destroy, this, &QWSceneNodePrivate::on_destroy);
    }
    ~QWSceneNodePrivate() {
        sc.invalidate();
        if (m_handle)
            wlr_scene_node_destroy(q_func()->handle());
    }

    void on_destroy(void *);

    QW_DECLARE_PUBLIC(QWSceneNode)
    QWSignalConnector sc;
};

void QWSceneNodePrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

QWSceneNode::QWSceneNode(wlr_scene_node *handle)
    : QWSceneNode(*new QWSceneNodePrivate(handle, this))
{

}

QWSceneNode::QWSceneNode(QWSceneNodePrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{

}

void QWSceneNode::setEnabled(bool enabled)
{
    wlr_scene_node_set_enabled(handle(), enabled);
}

void QWSceneNode::setPosition(const QPoint &pos)
{
    wlr_scene_node_set_position(handle(), pos.x(), pos.y());
}

void QWSceneNode::placeAbove(wlr_scene_node *sibling)
{
    wlr_scene_node_place_above(handle(), sibling);
}

void QWSceneNode::placeBelow(wlr_scene_node *sibling)
{
    wlr_scene_node_place_below(handle(), sibling);
}

void QWSceneNode::raiseToTop()
{
    wlr_scene_node_raise_to_top(handle());
}

void QWSceneNode::lowerToBottom()
{
    wlr_scene_node_lower_to_bottom(handle());
}

void QWSceneNode::reparent(wlr_scene_tree *newParent)
{
    wlr_scene_node_reparent(handle(), newParent);
}

bool QWSceneNode::coords(QPoint &pos) const
{
    return wlr_scene_node_coords(handle(), &pos.rx(), &pos.ry());
}

void QWSceneNode::forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *userData) const
{
    wlr_scene_node_for_each_buffer(handle(), iterator, userData);
}

wlr_scene_node *QWSceneNode::at(const QPointF &lpos, QPointF *npos) const
{
    return wlr_scene_node_at(handle(), lpos.x(), lpos.y(),
                             npos ? &npos->rx() : nullptr, npos ? &npos->ry() : nullptr);
}

QWSceneTree::QWSceneTree(wlr_scene_tree *handle)
    : QWSceneTree(*new QWSceneNodePrivate(&handle->node, this))
{

}

QWSceneTree::QWSceneTree(QWSceneNodePrivate &dd)
    : QWSceneNode(dd)
{

}

wlr_scene_tree *QWSceneTree::fromNode(wlr_scene_node *node)
{
    Q_ASSERT(node->type == WLR_SCENE_NODE_TREE);
    wlr_scene_tree *tree = wl_container_of(node, tree, node);
    return tree;
}

QWSceneTree *QWSceneTree::create(QWSceneTree *parent)
{
    auto handle = wlr_scene_tree_create(parent->handle());
    if (!handle)
        return nullptr;
    return new QWSceneTree(handle);
}

QWSceneTree *QWSceneTree::subsurfaceTreeCreate(QWSceneTree *parent, wlr_surface *surface)
{
    auto handle = wlr_scene_subsurface_tree_create(parent->handle(), surface);
    if (!handle)
        return nullptr;
    return new QWSceneTree(handle);
}

QWSceneTree *QWSceneTree::xdgSurfaceCreate(QWSceneTree *parent, wlr_xdg_surface *xdgSurface)
{
    auto handle = wlr_scene_xdg_surface_create(parent->handle(), xdgSurface);
    if (!handle)
        return nullptr;
    return new QWSceneTree(handle);
}

QWScene::QWScene(wlr_scene *handle)
    : QWSceneTree(*new QWSceneNodePrivate(&handle->tree.node, this))
{

}

wlr_scene *QWScene::handle() const
{
    auto node = QWSceneNode::handle();
    Q_ASSERT(node->type == WLR_SCENE_NODE_TREE);
    wlr_scene_tree *treeHandle = wl_container_of(node, treeHandle, node);
    wlr_scene *handle = wl_container_of(treeHandle, handle, tree);
    return handle;
}

QWScene *QWScene::create()
{
    auto handle = wlr_scene_create();
    if (!handle)
        return nullptr;
    return new QWScene(handle);
}

void QWScene::setPresentation(wlr_presentation *presentation)
{
    wlr_scene_set_presentation(handle(), presentation);
}

bool QWScene::attachOutputLayout(QWOutputLayout *outputLayout)
{
    return wlr_scene_attach_output_layout(handle(), outputLayout->handle());
}

class QWSceneBufferPrivate : public QWSceneNodePrivate
{
public:
    QWSceneBufferPrivate(void *handle, QWSceneBuffer *qq)
        : QWSceneNodePrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.output_enter, this, &QWSceneBufferPrivate::on_output_enter);
        sc.connect(&qq->handle()->events.output_leave, this, &QWSceneBufferPrivate::on_output_leave);
        sc.connect(&qq->handle()->events.output_present, this, &QWSceneBufferPrivate::on_output_present);
        sc.connect(&qq->handle()->events.frame_done, this, &QWSceneBufferPrivate::on_frame_done);
    }

    void on_output_enter(void *data);
    void on_output_leave(void *data);
    void on_output_present(void *data);
    void on_frame_done(void *data);

    QW_DECLARE_PUBLIC(QWSceneBuffer)
};

void QWSceneBufferPrivate::on_output_enter(void *data)
{
    Q_EMIT q_func()->outputEnter(reinterpret_cast<wlr_scene_output*>(data));
}

void QWSceneBufferPrivate::on_output_leave(void *data)
{
    Q_EMIT q_func()->outputLeave(reinterpret_cast<wlr_scene_output*>(data));
}

void QWSceneBufferPrivate::on_output_present(void *data)
{
    Q_EMIT q_func()->outputPresent(reinterpret_cast<wlr_scene_output*>(data));
}

void QWSceneBufferPrivate::on_frame_done(void *data)
{
    Q_EMIT q_func()->frameDone(reinterpret_cast<timespec*>(data));
}

QWSceneBuffer::QWSceneBuffer(wlr_scene_buffer *handle)
    : QWSceneNode(*new QWSceneBufferPrivate(&handle->node, this))
{

}

wlr_scene_buffer *QWSceneBuffer::fromNode(wlr_scene_node *node)
{
    return wlr_scene_buffer_from_node(node);
}

QWSceneBuffer *QWSceneBuffer::create(QWSceneTree *parent, wlr_buffer *buffer)
{
    auto handle = wlr_scene_buffer_create(parent->handle(), buffer);
    if (!handle)
        return nullptr;
    return new QWSceneBuffer(handle);
}

void QWSceneBuffer::setBuffer(QWBuffer *buffer)
{
    wlr_scene_buffer_set_buffer(handle(), buffer->handle());
}

void QWSceneBuffer::setBuffer(QWBuffer *buffer, pixman_region32 *region)
{
    wlr_scene_buffer_set_buffer_with_damage(handle(), buffer->handle(), region);
}

void QWSceneBuffer::setOpaqueRegion(pixman_region32 *region)
{
    wlr_scene_buffer_set_opaque_region(handle(), region);
}

void QWSceneBuffer::setSourceBox(const QRectF &box)
{
    wlr_fbox fbox {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };
    wlr_scene_buffer_set_source_box(handle(), &fbox);
}

void QWSceneBuffer::setDestSize(const QSize &size)
{
    wlr_scene_buffer_set_dest_size(handle(), size.width(), size.height());
}

void QWSceneBuffer::setTransform(int transform)
{
    wlr_scene_buffer_set_transform(handle(), static_cast<wl_output_transform>(transform));
}

void QWSceneBuffer::sendFrameDone(timespec *now)
{
    wlr_scene_buffer_send_frame_done(handle(), now);
}

wlr_scene_rect *QWSceneRect::fromeNode(wlr_scene_node *node)
{
    Q_ASSERT(node->type == WLR_SCENE_NODE_RECT);
    wlr_scene_rect *rect = wl_container_of(node, rect, node);
    return rect;
}

QWSceneRect *QWSceneRect::create(QWSceneTree *parent, const QSize &size, const QColor &color)
{
    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };
    auto handle = wlr_scene_rect_create(parent->handle(), size.width(), size.height(), c);
    if (!handle)
        return nullptr;
    return new QWSceneRect(handle);
}

void QWSceneRect::setSize(const QSize &size)
{
    wlr_scene_rect_set_size(handle(), size.width(), size.height());
}

void QWSceneRect::setColor(const QColor &color)
{
    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };
    wlr_scene_rect_set_color(handle(), c);
}

class QWSceneOutputPrivate : public QWObjectPrivate
{
public:
    QWSceneOutputPrivate(void *handle, QWSceneOutput *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&qq->handle()->events.destroy, this, &QWSceneOutputPrivate::on_destroy);
    }
    ~QWSceneOutputPrivate() {
        sc.invalidate();
        if (m_handle)
            wlr_scene_output_destroy(q_func()->handle());
    }

    void on_destroy(void *);

    QW_DECLARE_PUBLIC(QWSceneOutput)
    QWSignalConnector sc;
};

void QWSceneOutputPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

QWSceneOutput::QWSceneOutput(wlr_scene_output *handle)
    : QWObject(*new QWSceneOutputPrivate(handle, this))
    , QObject(nullptr)
{

}

QWSceneOutput *QWSceneOutput::create(QWScene *scene, wlr_output *output)
{
    auto handle = wlr_scene_output_create(scene->handle(), output);
    if (!handle)
        return nullptr;
    return new QWSceneOutput(handle);
}

wlr_scene_output *QWSceneOutput::get(QWScene *scene, wlr_output *output)
{
    return wlr_scene_get_scene_output(scene->handle(), output);
}

void QWSceneOutput::commit()
{
    wlr_scene_output_commit(handle());
}

void QWSceneOutput::sendFrameDone(timespec *now)
{
    wlr_scene_output_send_frame_done(handle(), now);
}

void QWSceneOutput::forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *userData) const
{
    wlr_scene_output_for_each_buffer(handle(), iterator, userData);
}

QW_END_NAMESPACE
