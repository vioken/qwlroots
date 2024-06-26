#include "qwrendererinterface.h"
#include "qwbuffer.h"
#include "qwrenderer.h"
#include "qwtexture.h"
#include "qwconfig.h"
#include <QtGlobal>
#include <QGenericMatrix>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QRect>
extern "C" {
#include <math.h>
#define static
#include <wlr/render/interface.h>
#undef static
#include <wlr/render/wlr_renderer.h>
#include <libdrm/drm_fourcc.h>
#include <wlr/render/drm_format_set.h>
#include <wlr/util/box.h>
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#else
#endif
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#endif
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#else
#endif
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#endif
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#else
#endif
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
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
#else
#endif
