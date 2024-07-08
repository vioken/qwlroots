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

    // bool enabled
    QW_FUNC_MEMBER(scene_node, set_enabled)
    // int x, int y
    QW_FUNC_MEMBER(scene_node, set_position)
    // wlr_scene_node *sibling
    QW_FUNC_MEMBER(scene_node, place_above)
    // wlr_scene_node *sibling
    QW_FUNC_MEMBER(scene_node, place_below)
    // Empty parameters
    QW_FUNC_MEMBER(scene_node, raise_to_top)
    // Empty parameters
    QW_FUNC_MEMBER(scene_node, lower_to_bottom)
    // wlr_scene_tree *new_parent
    QW_FUNC_MEMBER(scene_node, reparent)
    // int *lx, int *ly
    QW_FUNC_MEMBER(scene_node, coords)
    // wlr_scene_buffer_iterator_func_t iterator, void *user_data
    QW_FUNC_MEMBER(scene_node, for_each_buffer)
    // double lx, double ly, double *nx, double *ny
    QW_FUNC_MEMBER(scene_node, at)

protected:
    // Empty parameters
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
    // wlr_scene_tree *parent
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

    // wlr_scene_tree *parent
    QW_FUNC_STATIC(scene_tree, create)
    // wlr_scene_tree *parent, wlr_surface *surface
    QW_FUNC_STATIC(scene, subsurface_tree_create)
    // wlr_scene_node *node, wlr_box *clip
    QW_FUNC_STATIC(scene, subsurface_tree_set_clip)
    // wlr_scene_tree *parent, wlr_xdg_surface *xdg_surface
    QW_FUNC_STATIC(scene, xdg_surface_create)
    // wlr_scene_tree *parent, wlr_drag_icon *drag_icon
    QW_FUNC_STATIC(scene, drag_icon_create)

#if WLR_VERSION_MINOR<18
    // wlr_presentation *presentation
    QW_FUNC_MEMBER(scene, set_presentation)
#endif
    // wlr_linux_dmabuf_v1 *linux_dmabuf_v1
    QW_FUNC_MEMBER(scene, set_linux_dmabuf_v1)
    // wlr_output *output
    QW_FUNC_MEMBER(scene, get_scene_output)
    // wlr_output_layout *output_layout
    QW_FUNC_MEMBER(scene, attach_output_layout)
};

class qw_scene_surface
{
public:
    // wlr_scene_tree *parent, wlr_surface *surface
    QW_FUNC_STATIC(scene_surface, create)
    // wlr_scene_buffer *scene_buffer
    QW_FUNC_STATIC(scene_surface, try_from_buffer)
};

class qw_scene_timer
{
public:
    // Empty parameters
    QW_FUNC_MEMBER(scene_timer, get_duration_ns)
    // Empty parameters
    QW_FUNC_MEMBER(scene_timer, finish)
};

class QW_CLASS_REINTERPRET_CAST(scene_layer_surface_v1)
{
public:
    // wlr_scene_tree *parent, wlr_layer_surface_v1 *layer_surface
    QW_FUNC_STATIC(scene_layer_surface_v1, create)

    // const wlr_box *full_area, wlr_box *usable_area
    QW_FUNC_MEMBER(scene_layer_surface_v1, configure)
};

class qw_scene_rect : public qw_scene_node
{
    Q_OBJECT
    QW_SCENE_NODE(scene_rect)

public:
    // wlr_scene_tree *parent, int width, int height, const float color[4]
    QW_FUNC_STATIC(scene_rect, create)

    // int width, int height
    QW_FUNC_MEMBER(scene_rect, set_size)
    // const float color[4]
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
    // wlr_scene_tree *parent, wlr_buffer *buffer
    QW_FUNC_STATIC(scene_buffer, create)

    // wlr_buffer *buffer
    QW_FUNC_MEMBER(scene_buffer, set_buffer)
    // wlr_buffer *buffer, const pixman_region32_t *region
    QW_FUNC_MEMBER(scene_buffer, set_buffer_with_damage)
    // const pixman_region32_t *region
    QW_FUNC_MEMBER(scene_buffer, set_opaque_region)
    // const wlr_fbox *box
    QW_FUNC_MEMBER(scene_buffer, set_source_box)
    // int width, int height
    QW_FUNC_MEMBER(scene_buffer, set_dest_size)
    // enum wl_output_transform transform
    QW_FUNC_MEMBER(scene_buffer, set_transform)
    // float opacity
    QW_FUNC_MEMBER(scene_buffer, set_opacity)
    // enum wlr_scale_filter_mode filter_mode
    QW_FUNC_MEMBER(scene_buffer, set_filter_mode)
    // timespec *now
    QW_FUNC_MEMBER(scene_buffer, send_frame_done)
};

class qw_scene_output : public qw_scene_node
{
    Q_OBJECT

public:
    // wlr_scene *scene, wlr_output *output
    QW_FUNC_STATIC(scene_output, create)
    // wlr_scene_output_layout *sol, wlr_output_layout_output *lo, wlr_scene_output *so
    QW_FUNC_STATIC(scene_output, layout_add_output)

    // int lx, int ly
    QW_FUNC_MEMBER(scene_output, set_position)
    // const wlr_scene_output_state_options *options
    QW_FUNC_MEMBER(scene_output, commit)
    // wlr_output_state *state, const wlr_scene_output_state_options *options
    QW_FUNC_MEMBER(scene_output, build_state)
    // timespec *now
    QW_FUNC_MEMBER(scene_output, send_frame_done)
    // wlr_scene_buffer_iterator_func_t iterator, void *user_data
    QW_FUNC_MEMBER(scene_output, for_each_buffer)

protected:
    // Empty parameters
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
