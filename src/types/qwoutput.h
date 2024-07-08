// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(output)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(frame)
    QW_SIGNAL(damage, wlr_output_event_damage*)
    QW_SIGNAL(needs_frame)
    QW_SIGNAL(precommit, wlr_output_event_precommit*)
    QW_SIGNAL(commit, wlr_output_event_commit*)
    QW_SIGNAL(present, wlr_output_event_present*)
    QW_SIGNAL(bind, wlr_output_event_bind*)
    QW_SIGNAL(description)
    QW_SIGNAL(request_state, wlr_output_event_request_state*)

public:
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    // bool enable
    QW_FUNC_MEMBER(output, enable)
#endif
    // Empty parameters
    QW_FUNC_MEMBER(output, create_global)
    // Empty parameters
    QW_FUNC_MEMBER(output, destroy_global)
    // wlr_allocator *allocator, wlr_renderer *renderer
    QW_FUNC_MEMBER(output, init_render)
    // Empty parameters
    QW_FUNC_MEMBER(output, preferred_mode)

#if WLR_VERSION_MINOR < 18
    // wlr_output_mode *mode
    QW_FUNC_MEMBER(output, set_mode)
    // int32_t width, int32_t height, int32_t refresh
    QW_FUNC_MEMBER(output, set_custom_mode)
    // enum wl_output_transform transform
    QW_FUNC_MEMBER(output, set_transform)
    // bool enabled
    QW_FUNC_MEMBER(output, enable_adaptive_sync)
    // uint32_t format
    QW_FUNC_MEMBER(output, set_render_format)
    // float scale
    QW_FUNC_MEMBER(output, set_scale)
    // enum wl_output_subpixel subpixel
    QW_FUNC_MEMBER(output, set_subpixel)
    // const pixman_region32_t *damage
    QW_FUNC_MEMBER(output, set_damage)
#endif
    // const char *name
    QW_FUNC_MEMBER(output, set_name)
    // const char *desc
    QW_FUNC_MEMBER(output, set_description)
    // Empty parameters
    QW_FUNC_MEMBER(output, schedule_done)
    // int *width, int *height
    QW_FUNC_MEMBER(output, transformed_resolution)
    // int *width, int *height
    QW_FUNC_MEMBER(output, effective_resolution)

#if WLR_VERSION_MINOR < 18
    // int *buffer_age
    QW_FUNC_MEMBER(output, attach_render)
    // wlr_buffer *buffer
    QW_FUNC_MEMBER(output, attach_buffer)
    // Empty parameters
    QW_FUNC_MEMBER(output, preferred_read_format)
#endif

    // bool lock
    QW_FUNC_MEMBER(output, lock_attach_render)

#if WLR_VERSION_MINOR < 18
    // Empty parameters
    QW_FUNC_MEMBER(output, test)
    // Empty parameters
    QW_FUNC_MEMBER(output, commit)
    // Empty parameters
    QW_FUNC_MEMBER(output, rollback)
    // size_t size, const uint16_t *r, const uint16_t *g, const uint16_t *b
    QW_FUNC_MEMBER(output, set_gamma)
#endif

    // const wlr_output_state *state
    QW_FUNC_MEMBER(output, test_state)
    // const wlr_output_state *state
    QW_FUNC_MEMBER(output, commit_state)

    // Empty parameters
    QW_FUNC_MEMBER(output, schedule_frame)
    // Empty parameters
    QW_FUNC_MEMBER(output, get_gamma_size)
    // bool lock
    QW_FUNC_MEMBER(output, lock_software_cursors)

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    // const pixman_region32_t *damage
    QW_FUNC_MEMBER(output, render_software_cursors)
#endif

    // uint32_t buffer_caps
    QW_FUNC_MEMBER(output, get_primary_formats)
    // wlr_render_pass *render_pass, const pixman_region32_t *damage
    QW_FUNC_MEMBER(output, add_software_cursors_to_render_pass)
    // const wlr_output_state *state, wlr_swapchain **swapchain
    QW_FUNC_MEMBER(output, configure_primary_swapchain)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(output, destroy)
};

class QW_CLASS_REINTERPRET_CAST(output_cursor)
{
public:
    // wlr_output *output
    QW_FUNC_STATIC(output_cursor, create)

    // wlr_buffer *buffer, int32_t hotspot_x, int32_t hotspot_y
    QW_FUNC_MEMBER(output_cursor, set_buffer)
    // double x, double y
    QW_FUNC_MEMBER(output_cursor, move)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(output_cursor, destroy)
};

class QW_CLASS_REINTERPRET_CAST(output_state)
{
public:
    // Empty parameters
    QW_FUNC_MEMBER(output_state, init)
    // Empty parameters
    QW_FUNC_MEMBER(output_state, finish)
    // bool enabled
    QW_FUNC_MEMBER(output_state, set_enabled)
    // wlr_output_mode *mode
    QW_FUNC_MEMBER(output_state, set_mode)
    // float scale
    QW_FUNC_MEMBER(output_state, set_scale)
    // enum wl_output_transform transform
    QW_FUNC_MEMBER(output_state, set_transform)
    // bool enabled
    QW_FUNC_MEMBER(output_state, set_adaptive_sync_enabled)
    // uint32_t format
    QW_FUNC_MEMBER(output_state, set_render_format)
    // enum wl_output_subpixel subpixel
    QW_FUNC_MEMBER(output_state, set_subpixel)
    // wlr_buffer *buffer
    QW_FUNC_MEMBER(output_state, set_buffer)
    // size_t ramp_size, const uint16_t *r, const uint16_t *g, const uint16_t *b
    QW_FUNC_MEMBER(output_state, set_gamma_lut)
    // const pixman_region32_t *damage
    QW_FUNC_MEMBER(output_state, set_damage)
    // wlr_output_layer_state *layers, size_t layers_len
    QW_FUNC_MEMBER(output_state, set_layers)
    // const wlr_output_state *src
    QW_FUNC_MEMBER(output_state, copy)
};

QW_END_NAMESPACE
