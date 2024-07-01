// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wayland-server-core.h>
#define static
#include <wlr/render/wlr_renderer.h>
#if WLR_VERSION_MINOR >= 18
#include <wlr/render/pass.h>
#endif
#undef static
#include <wlr/util/box.h>
}

struct wlr_renderer;
struct wlr_box;
struct wlr_fbox;
struct wlr_drm_format_set;
struct wl_display;
struct wlr_render_texture_options;

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(renderer)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(lost)

public:
    QW_FUNC_MEMBER(renderer, autocreate)
    QW_FUNC_MEMBER(renderer, init_wl_display)
    QW_FUNC_MEMBER(renderer, init_wl_shm)
    QW_FUNC_MEMBER(renderer, get_drm_fd)

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    QW_FUNC_MEMBER(renderer, begin)
    QW_FUNC_MEMBER(renderer, end)
    QW_FUNC_MEMBER(renderer, begin_buffer_pass)
    QW_FUNC_MEMBER(renderer, begin_with_buffer)
    QW_FUNC_MEMBER(renderer, clear)
    QW_FUNC_MEMBER(renderer, scissor)
    QW_FUNC_MEMBER(render, texture)
    QW_FUNC_MEMBER(render, texture_with_matrix)
    QW_FUNC_MEMBER(render, subtexture_with_matrix)
    QW_FUNC_MEMBER(render, rect)
    QW_FUNC_MEMBER(render, quad_with_matrix)
    QW_FUNC_MEMBER(renderer, get_shm_texture_formats)
    QW_FUNC_MEMBER(renderer, get_dmabuf_texture_formats)
    QW_FUNC_MEMBER(renderer, read_pixels)
#else // WLR_VERSION_MINOR >= 18
    QW_FUNC_MEMBER(renderer, get_texture_formats)
#endif

private:
    QW_FUNC_MEMBER(renderer, destroy)
};

QW_END_NAMESPACE
