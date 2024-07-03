// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_compositor.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(compositor)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_surface, wlr_surface*)

public:
    QW_FUNC_STATIC(compositor, create)
};

class QW_CLASS_OBJECT(surface)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(client_commit)
    QW_SIGNAL(commit)
    QW_SIGNAL(new_subsurface, wlr_subsurface*)
#if WLR_VERSION_MINOR < 18
    QW_SIGNAL(precommit, wlr_surface_state*)
#endif
    QW_SIGNAL(map)
    QW_SIGNAL(unmap)

public:
    QW_FUNC_STATIC(surface, from_resource)

    QW_FUNC_MEMBER(surface, for_each_surface)
    QW_FUNC_MEMBER(surface, get_buffer_source_box)
    QW_FUNC_MEMBER(surface, get_effective_damage)
    QW_FUNC_MEMBER(surface, get_extends)
    QW_FUNC_MEMBER(surface, get_root_surface)
    QW_FUNC_MEMBER(surface, get_texture)
    QW_FUNC_MEMBER(surface, has_buffer)
    QW_FUNC_MEMBER(surface, lock_pending)
    QW_FUNC_MEMBER(surface, point_accepts_input)
    QW_FUNC_MEMBER(surface, send_enter)
    QW_FUNC_MEMBER(surface, send_frame_done)
    QW_FUNC_MEMBER(surface, send_leave)
    QW_FUNC_MEMBER(surface, surface_at)
    QW_FUNC_MEMBER(surface, unlock_cached)
    QW_FUNC_MEMBER(surface, set_preferred_buffer_scale)
    QW_FUNC_MEMBER(surface, set_preferred_buffer_transform)
    QW_FUNC_MEMBER(surface, set_role)
    QW_FUNC_MEMBER(surface, map)
    QW_FUNC_MEMBER(surface, unmap)
};

QW_END_NAMESPACE
