#include "qwrenderer.h"
#include "qwbackend.h"
#include "qwdisplay.h"
#include "qwtexture.h"
#include "types/qwbuffer.h"
#include "private/qwglobal_p.h"
#include <QColor>
#include <QRect>
#include <QHash>
extern "C" {
#include <wayland-server-core.h>
#define static
#include <wlr/render/wlr_renderer.h>
#if WLR_VERSION_MINOR >= 18
#include <wlr/render/pass.h>
#endif
#undef static
#include <wlr/util/box.h>
QW_SIGNAL(lost, arguments?)
QW_FUNC_MEMBER(renderer, autocreate)
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(renderer, begin)
#endif
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(renderer, begin_with_buffer)
# else
QW_FUNC_MEMBER(renderer, begin_buffer_pass)
#endif
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(render, pass_submit)
#else
QW_FUNC_MEMBER(renderer, end)
#endif
QW_FUNC_MEMBER(renderer, init_wl_display)
QW_FUNC_MEMBER(renderer, init_wl_shm)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
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
QW_FUNC_MEMBER(renderer, get_drm_fd)
