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
    QW_FUNC_MEMBER(output, enable)
#endif
    QW_FUNC_MEMBER(output, create_global)
    QW_FUNC_MEMBER(output, destroy_global)
    QW_FUNC_MEMBER(output, init_render)
    QW_FUNC_MEMBER(output, preferred_mode)

#if WLR_VERSION_MINOR < 18
    QW_FUNC_MEMBER(output, set_mode)
    QW_FUNC_MEMBER(output, set_custom_mode)
    QW_FUNC_MEMBER(output, set_transform)
    QW_FUNC_MEMBER(output, enable_adaptive_sync)
    QW_FUNC_MEMBER(output, set_render_format)
    QW_FUNC_MEMBER(output, set_scale)
    QW_FUNC_MEMBER(output, set_subpixel)
    QW_FUNC_MEMBER(output, set_damage)
#endif
    QW_FUNC_MEMBER(output, set_name)
    QW_FUNC_MEMBER(output, set_description)
    QW_FUNC_MEMBER(output, schedule_done)
    QW_FUNC_MEMBER(output, transformed_resolution)
    QW_FUNC_MEMBER(output, effective_resolution)

#if WLR_VERSION_MINOR < 18
    QW_FUNC_MEMBER(output, attach_render)
    QW_FUNC_MEMBER(output, attach_buffer)
    QW_FUNC_MEMBER(output, preferred_read_format)
#endif

    QW_FUNC_MEMBER(output, lock_attach_render)

#if WLR_VERSION_MINOR < 18
    QW_FUNC_MEMBER(output, test)
    QW_FUNC_MEMBER(output, commit)
    QW_FUNC_MEMBER(output, rollback)
    QW_FUNC_MEMBER(output, set_gamma)
#endif

    QW_FUNC_MEMBER(output, test_state)
    QW_FUNC_MEMBER(output, commit_state)

    QW_FUNC_MEMBER(output, schedule_frame)
    QW_FUNC_MEMBER(output, get_gamma_size)
    QW_FUNC_MEMBER(output, lock_software_cursors)

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    QW_FUNC_MEMBER(output, render_software_cursors)
#endif

    QW_FUNC_MEMBER(output, get_primary_formats)
    QW_FUNC_MEMBER(output, add_software_cursors_to_render_pass)
    QW_FUNC_MEMBER(output, configure_primary_swapchain)

protected:
    QW_FUNC_MEMBER(output, destroy)
};

class QW_CLASS_REINTERPRET_CAST(output_cursor)
{
public:
    QW_FUNC_STATIC(output_cursor, create)

    QW_FUNC_MEMBER(output_cursor, set_buffer)
    QW_FUNC_MEMBER(output_cursor, move)

protected:
    QW_FUNC_MEMBER(output_cursor, destroy)
};

class QW_CLASS_REINTERPRET_CAST(output_state)
{
public:
    QW_FUNC_MEMBER(output_state, init)
    QW_FUNC_MEMBER(output_state, finish)
    QW_FUNC_MEMBER(output_state, set_enabled)
    QW_FUNC_MEMBER(output_state, set_mode)
    QW_FUNC_MEMBER(output_state, set_scale)
    QW_FUNC_MEMBER(output_state, set_transform)
    QW_FUNC_MEMBER(output_state, set_adaptive_sync_enabled)
    QW_FUNC_MEMBER(output_state, set_render_format)
    QW_FUNC_MEMBER(output_state, set_subpixel)
    QW_FUNC_MEMBER(output_state, set_buffer)
    QW_FUNC_MEMBER(output_state, set_gamma_lut)
    QW_FUNC_MEMBER(output_state, set_damage)
    QW_FUNC_MEMBER(output_state, set_layers)
    QW_FUNC_MEMBER(output_state, copy)
};

QW_END_NAMESPACE
