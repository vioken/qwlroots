// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

extern "C" {
#define static
#include <wlr/render/interface.h>
#undef static
#include <wlr/render/wlr_renderer.h>
#include <libdrm/drm_fourcc.h>
#include <wlr/render/drm_format_set.h>
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

template<typename Derive>
class QW_CLASS_INTERFACE(renderer)
{
    QW_INTERFACE_INIT(renderer)

public:
    QW_INTERFACE_FUNC_STATIC(renderer, init, void, wlr_renderer *renderer, const wlr_renderer_impl *impl)

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    QW_INTERFACE(bind_buffer)
    QW_INTERFACE(begin)
    QW_INTERFACE(end)
    QW_INTERFACE(clear)
    QW_INTERFACE(scissor)
    QW_INTERFACE(render_subtexture_with_matrix)
    QW_INTERFACE(render_quad_with_matrix)
    QW_INTERFACE(get_shm_texture_formats)
    QW_INTERFACE(get_dmabuf_texture_formats)
#else // WLR_VERSION_MINOR >= 18
    QW_INTERFACE(get_texture_formats)
#endif
    QW_INTERFACE(get_render_formats)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    QW_INTERFACE(preferred_read_format)
    QW_INTERFACE(read_pixels)
#endif
    QW_INTERFACE(destroy)
    QW_INTERFACE(get_drm_fd)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    QW_INTERFACE(get_render_buffer_caps)
#endif
    QW_INTERFACE(texture_from_buffer)
    QW_INTERFACE(begin_buffer_pass)
    QW_INTERFACE(render_timer_create)
};

QW_END_NAMESPACE
