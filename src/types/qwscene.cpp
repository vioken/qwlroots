// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwscene.h"
#include "qwoutputlayout.h"
#include "qwbuffer.h"
#include "qwxdgshell.h"
#include "qwoutput.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"

#include <QColor>
#include <QPoint>
#include <QRect>
#include <QHash>

extern "C" {
// avoid replace static
#include <math.h>
#define static
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/util/box.h>
}

static_assert(std::is_same_v<wl_output_transform_t, std::underlying_type_t<wl_output_transform>>);

QW_BEGIN_NAMESPACE

class QWSceneNodePrivate : public QWObjectPrivate
{
public:
    QWSceneNodePrivate(wlr_scene_node *handle, bool isOwner, QWSceneNode *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSceneNodePrivate::on_destroy);
    }
    ~QWSceneNodePrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_scene_node_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWSceneNode*> map;
    QW_DECLARE_PUBLIC(QWSceneNode)
    QWSignalConnector sc;
};
QHash<void*, QWSceneNode*> QWSceneNodePrivate::map;

void QWSceneNodePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWSceneNode::QWSceneNode(wlr_scene_node *handle, bool isOwner)
    : QWSceneNode(*new QWSceneNodePrivate(handle, isOwner, this))
{

}

QWSceneNode::QWSceneNode(QWSceneNodePrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{

}

QWSceneNode *QWSceneNode::get(wlr_scene_node *handle)
{
    return QWSceneNodePrivate::map.value(handle);
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

QWSceneTree::QWSceneTree(wlr_scene_tree *handle, bool isOwner)
    : QWSceneTree(*new QWSceneNodePrivate(&handle->node, isOwner, this))
{

}

QWSceneTree::QWSceneTree(QWSceneNodePrivate &dd)
    : QWSceneNode(dd)
{

}

QWSceneTree::QWSceneTree(QWSceneTree *parent)
    : QWSceneTree(wlr_scene_tree_create(parent->handle()), true)
{

}

wlr_scene_tree *QWSceneTree::handle() const
{
    auto node = QWSceneNode::handle();
    Q_ASSERT(node->type == WLR_SCENE_NODE_TREE);
    wlr_scene_tree *tree = wl_container_of(node, tree, node);
    return tree;
}

QWSceneTree *QWSceneTree::get(wlr_scene_tree *handle)
{
    auto node = QWSceneNode::get(&handle->node);
    if (!node)
        return nullptr;
    auto o = qobject_cast<QWSceneTree*>(node);
    Q_ASSERT(o);
    return o;
}

QWSceneTree *QWSceneTree::from(wlr_scene_tree *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSceneTree(handle, false);
}

QWSceneTree *QWSceneTree::from(wlr_scene_node *node)
{
    Q_ASSERT(node->type == WLR_SCENE_NODE_TREE);
    wlr_scene_tree *tree = wl_container_of(node, tree, node);
    return from(tree);
}

QWSceneTree *QWSceneTree::subsurfaceTreeCreate(QWSceneTree *parent, QWSurface *surface)
{
    auto handle = wlr_scene_subsurface_tree_create(parent->handle(), surface->handle());
    if (!handle)
        return nullptr;
    return new QWSceneTree(handle, true);
}

QWSceneTree *QWSceneTree::xdgSurfaceCreate(QWSceneTree *parent, QWXdgSurface *xdgSurface)
{
    auto handle = wlr_scene_xdg_surface_create(parent->handle(), xdgSurface->handle());
    if (!handle)
        return nullptr;
    return new QWSceneTree(handle, true);
}

QWScene::QWScene(wlr_scene *handle, bool isOwner, QObject *parent)
    : QWSceneTree(*new QWSceneNodePrivate(&handle->tree.node, isOwner, this))
{

}

QWScene::QWScene(QObject *parent)
    : QWScene(wlr_scene_create(), true, parent)
{

}

wlr_scene *QWScene::handle() const
{
    wlr_scene_tree *treeHandle = QWSceneTree::handle();
    wlr_scene *handle = wl_container_of(treeHandle, handle, tree);
    return handle;
}

QWScene *QWScene::get(wlr_scene *handle)
{
    auto tree = QWSceneTree::get(&handle->tree);
    if (!tree)
        return nullptr;
    auto o = qobject_cast<QWScene*>(tree);
    Q_ASSERT(o);
    return o;
}

QWScene *QWScene::from(wlr_scene *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWScene(handle, true, nullptr);
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
    QWSceneBufferPrivate(wlr_scene_buffer *handle, bool isOwner, QWSceneBuffer *qq)
        : QWSceneNodePrivate(&handle->node, isOwner, qq)
    {
        sc.connect(&handle->events.output_enter, this, &QWSceneBufferPrivate::on_output_enter);
        sc.connect(&handle->events.output_leave, this, &QWSceneBufferPrivate::on_output_leave);
#if WLR_VERSION_MINOR > 16
        sc.connect(&handle->events.output_sample, this, &QWSceneBufferPrivate::on_output_sample);
#else
        sc.connect(&handle->events.output_present, this, &QWSceneBufferPrivate::on_output_present);
#endif
        sc.connect(&handle->events.frame_done, this, &QWSceneBufferPrivate::on_frame_done);
    }

    void on_output_enter(void *data);
    void on_output_leave(void *data);
#if WLR_VERSION_MINOR > 16
    void on_output_sample(void *data);
#else
    void on_output_present(void *data);
#endif
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

#if WLR_VERSION_MINOR > 16
void QWSceneBufferPrivate::on_output_sample(void *data)
{
    Q_EMIT q_func()->outputSample(reinterpret_cast<wlr_scene_output_sample_event*>(data));
}
#else
void QWSceneBufferPrivate::on_output_present(void *data)
{
    Q_EMIT q_func()->outputPresent(reinterpret_cast<wlr_scene_output*>(data));
}
#endif

void QWSceneBufferPrivate::on_frame_done(void *data)
{
    Q_EMIT q_func()->frameDone(reinterpret_cast<timespec*>(data));
}

QWSceneBuffer::QWSceneBuffer(wlr_scene_buffer *handle, bool isOwner)
    : QWSceneNode(*new QWSceneBufferPrivate(handle, isOwner, this))
{

}

QWSceneBuffer::QWSceneBuffer(QWBuffer *buffer, QWSceneTree *parent)
    : QWSceneBuffer(wlr_scene_buffer_create(parent->handle(), buffer->handle()), true)
{

}

wlr_scene_buffer *QWSceneBuffer::handle() const
{
    return wlr_scene_buffer_from_node(QWSceneNode::handle());
}

QWSceneBuffer *QWSceneBuffer::get(wlr_scene_buffer *handle)
{
    auto node = QWSceneNode::get(&handle->node);
    if (!node)
        return nullptr;
    auto o = qobject_cast<QWSceneBuffer*>(node);
    Q_ASSERT(o);
    return o;
}

QWSceneBuffer *QWSceneBuffer::from(wlr_scene_buffer *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSceneBuffer(handle, false);
}

QWSceneBuffer *QWSceneBuffer::from(wlr_scene_node *node)
{
    return from(wlr_scene_buffer_from_node(node));
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

void QWSceneBuffer::setTransform(wl_output_transform_t transform)
{
    wlr_scene_buffer_set_transform(handle(), static_cast<wl_output_transform>(transform));
}

void QWSceneBuffer::sendFrameDone(timespec *now)
{
    wlr_scene_buffer_send_frame_done(handle(), now);
}

QWSceneRect::QWSceneRect(wlr_scene_rect *handle, bool isOwner)
    : QWSceneNode(*new QWSceneNodePrivate(&handle->node, isOwner, this))
{

}

static inline wlr_scene_rect *createWlrSceneRect(QWSceneTree *parent, const QSize &size, const QColor &color)
{
    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };
    return wlr_scene_rect_create(parent->handle(), size.width(), size.height(), c);
}

QWSceneRect::QWSceneRect(const QSize &size, const QColor &color, QWSceneTree *parent)
    : QWSceneRect(createWlrSceneRect(parent, size, color), true)
{

}

QWSceneRect *QWSceneRect::get(wlr_scene_rect *handle)
{
    auto node = QWSceneNode::get(&handle->node);
    if (!node)
        return nullptr;
    auto o = qobject_cast<QWSceneRect*>(node);
    Q_ASSERT(o);
    return o;
}

QWSceneRect *QWSceneRect::from(wlr_scene_rect *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSceneRect(handle, false);
}

QWSceneRect *QWSceneRect::frome(wlr_scene_node *node)
{
    Q_ASSERT(node->type == WLR_SCENE_NODE_RECT);
    wlr_scene_rect *rect = wl_container_of(node, rect, node);
    return from(rect);
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
    QWSceneOutputPrivate(wlr_scene_output *handle, bool isOwner, QWSceneOutput *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSceneOutputPrivate::on_destroy);
    }
    ~QWSceneOutputPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_scene_output_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWSceneOutput*> map;
    QW_DECLARE_PUBLIC(QWSceneOutput)
    QWSignalConnector sc;
};
QHash<void*, QWSceneOutput*> QWSceneOutputPrivate::map;

void QWSceneOutputPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWSceneOutput::QWSceneOutput(wlr_scene_output *handle, bool isOwner)
    : QWObject(*new QWSceneOutputPrivate(handle, isOwner, this))
    , QObject(nullptr)
{

}

QWSceneOutput::QWSceneOutput(QWScene *scene, QWOutput *output)
    : QWSceneOutput(wlr_scene_output_create(scene->handle(), output->handle()), true)
{

}

QWSceneOutput *QWSceneOutput::get(wlr_scene_output *handle)
{
    return QWSceneOutputPrivate::map.value(handle);
}

QWSceneOutput *QWSceneOutput::from(wlr_scene_output *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSceneOutput(handle, false);
}

QWSceneOutput *QWSceneOutput::from(QWScene *scene, QWOutput *output)
{
    auto handle = wlr_scene_get_scene_output(scene->handle(), output->handle());
    if (!handle)
        return nullptr;
    return from(handle);
}

#if WLR_VERSION_MINOR > 16
void QWSceneOutput::commit(const wlr_scene_output_state_options *options)
{
    wlr_scene_output_commit(handle(), options);
}
#else
void QWSceneOutput::commit()
{
    wlr_scene_output_commit(handle());
}
#endif

void QWSceneOutput::sendFrameDone(timespec *now)
{
    wlr_scene_output_send_frame_done(handle(), now);
}

void QWSceneOutput::forEachBuffer(wlr_scene_buffer_iterator_func_t iterator, void *userData) const
{
    wlr_scene_output_for_each_buffer(handle(), iterator, userData);
}

QW_END_NAMESPACE
