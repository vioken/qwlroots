// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_scene.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(scene_node)
{
    QW_OBJECT
    Q_OBJECT

public:
    template<typename T>
    QW_ALWAYS_INLINE static std::enable_if<std::is_base_of<qw_scene_node, T>::value, qw_scene_node*>::type
    from(wlr_scene_node *handle) {
        Q_ASSERT(T::from_node(handle));
        return new T(handle, false);
    }

    inline static qw_scene_node *create(HandleType *handle);

    QW_FUNC_MEMBER(scene_node, set_enabled)
    QW_FUNC_MEMBER(scene_node, set_position)
    QW_FUNC_MEMBER(scene_node, place_above)
    QW_FUNC_MEMBER(scene_node, place_below)
    QW_FUNC_MEMBER(scene_node, raise_to_top)
    QW_FUNC_MEMBER(scene_node, lower_to_bottom)
    QW_FUNC_MEMBER(scene_node, reparent)
    QW_FUNC_MEMBER(scene_node, coords)
    QW_FUNC_MEMBER(scene_node, for_each_buffer)
    QW_FUNC_MEMBER(scene_node, at)

protected:
    QW_FUNC_MEMBER(scene_node, destroy)
};

#define QW_SCENE_NODE(name) \
typedef qw_##name DeriveType; \
public: \
    QW_ALWAYS_INLINE wlr_##name *handle() const { \
        return from_node(qw_scene_node::handle()); \
    } \
    QW_ALWAYS_INLINE operator wlr_##name* () const { \
        return handle(); \
    } \
    QW_FUNC_STATIC(name, from_node) \
protected: \
using qw_scene_node::qw_scene_node; \
private: \
friend class qw_scene_node;

class qw_scene_tree : public qw_scene_node
{
    Q_OBJECT
    QW_SCENE_NODE(scene_tree)

public:
    QW_FUNC_STATIC(scene_tree, create)
};

class qw_scene : public qw_scene_tree
{
    Q_OBJECT

public:
    QW_ALWAYS_INLINE wlr_scene *handle() const {
       wlr_scene *handle = wl_container_of(qw_scene_tree::handle(), handle, tree);
       return handle;
    }
    QW_ALWAYS_INLINE operator wlr_scene* () const {
        return handle();
    }

    QW_FUNC_STATIC(scene_tree, create)
    QW_FUNC_STATIC(scene, subsurface_tree_create)
    QW_FUNC_STATIC(scene, subsurface_tree_set_clip)
    QW_FUNC_STATIC(scene, xdg_surface_create)
    QW_FUNC_STATIC(scene, drag_icon_create)

#if WLR_VERSION_MINOR<18
    QW_FUNC_MEMBER(scene, set_presentation)
#endif
    QW_FUNC_MEMBER(scene, set_linux_dmabuf_v1)
    QW_FUNC_MEMBER(scene, get_scene_output)
    QW_FUNC_MEMBER(scene, attach_output_layout)
};

class qw_scene_surface
{
public:
    QW_FUNC_STATIC(scene_surface, create)
    QW_FUNC_STATIC(scene_surface, try_from_buffer)
};

class qw_scene_timer
{
public:
    QW_FUNC_MEMBER(scene_timer, get_duration_ns)
    QW_FUNC_MEMBER(scene_timer, finish)
};

class QW_CLASS_REINTERPRET_CAST(scene_layer_surface_v1)
{
public:
    QW_FUNC_STATIC(scene_layer_surface_v1, create)

    QW_FUNC_MEMBER(scene_layer_surface_v1, configure)
};

class qw_scene_rect : public qw_scene_node
{
    Q_OBJECT
    QW_SCENE_NODE(scene_rect)

public:
    QW_FUNC_STATIC(scene_rect, create)

    QW_FUNC_MEMBER(scene_rect, set_size)
    QW_FUNC_MEMBER(scene_rect, set_color)
};

class qw_scene_buffer : public qw_scene_node
{
    Q_OBJECT
    QW_SCENE_NODE(scene_buffer)

    QW_SIGNAL(outputs_update, wlr_scene_outputs_update_event*)
    QW_SIGNAL(output_enter, wlr_scene_output*)
    QW_SIGNAL(output_leave, wlr_scene_output*)
    QW_SIGNAL(output_sample, wlr_scene_output_sample_event*)
    QW_SIGNAL(frame_done, timespec*)

public:
    QW_FUNC_STATIC(scene_buffer, create)

    QW_FUNC_MEMBER(scene_buffer, set_buffer)
    QW_FUNC_MEMBER(scene_buffer, set_buffer_with_damage)
    QW_FUNC_MEMBER(scene_buffer, set_opaque_region)
    QW_FUNC_MEMBER(scene_buffer, set_source_box)
    QW_FUNC_MEMBER(scene_buffer, set_dest_size)
    QW_FUNC_MEMBER(scene_buffer, set_transform)
    QW_FUNC_MEMBER(scene_buffer, set_opacity)
    QW_FUNC_MEMBER(scene_buffer, set_filter_mode)
    QW_FUNC_MEMBER(scene_buffer, send_frame_done)
};

class qw_scene_output : public qw_scene_node
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(scene_output, create)
    QW_FUNC_STATIC(scene_output, layout_add_output)

    QW_FUNC_MEMBER(scene_output, set_position)
    QW_FUNC_MEMBER(scene_output, commit)
    QW_FUNC_MEMBER(scene_output, build_state)
    QW_FUNC_MEMBER(scene_output, send_frame_done)
    QW_FUNC_MEMBER(scene_output, for_each_buffer)

protected:
    QW_FUNC_MEMBER(scene_output, destroy)
};

qw_scene_node *qw_scene_node::create(HandleType *handle) {
    if (auto o = get(handle))
        return o;

    switch (handle->type) {
    case WLR_SCENE_NODE_RECT:
        return from<qw_scene_rect>(handle);
    case WLR_SCENE_NODE_TREE:
        return from<qw_scene_tree>(handle);
    case WLR_SCENE_NODE_BUFFER:
        return from<qw_scene_buffer>(handle);
    default:
        // Here is not reachable
        qCritical("Unknow input device type!");
        return nullptr;
    }
}

QW_END_NAMESPACE
