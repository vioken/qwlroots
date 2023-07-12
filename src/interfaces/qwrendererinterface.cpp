// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
// FIXME: float i[static 3] not working c++
#define static
#include <wlr/render/interface.h>
#undef static
#include <wlr/render/wlr_renderer.h>
#include <libdrm/drm_fourcc.h>
#include <wlr/render/drm_format_set.h>
#include <wlr/util/box.h>
}

inline static int uint32ToPixelSize(uint32_t t) {
    Q_ASSERT_X(t <= INT_MAX, "QWRendererInterface", QString("%1 is greater than INT_MAX.").arg(t).toUtf8().data());
    return static_cast<int>(t);
}

static QImage::Format toQImageFormat(uint32_t format)
{
    switch (format) {
    /* 8bpp formats */
    case DRM_FORMAT_C8:
        return QImage::Format_Indexed8;

    /* 16 bpp RGB */
    case DRM_FORMAT_XRGB4444:
        return QImage::Format_RGB444;
    case DRM_FORMAT_XBGR4444:
        break;
    case DRM_FORMAT_RGBX4444:
        break;
    case DRM_FORMAT_BGRX4444:
        break;
    case DRM_FORMAT_ARGB4444:
        return QImage::Format_ARGB4444_Premultiplied;
    case DRM_FORMAT_ABGR4444:
        break;
    case DRM_FORMAT_RGBA4444:
        break;
    case DRM_FORMAT_BGRA4444:
        break;
    case DRM_FORMAT_XRGB1555:
        return QImage::Format_RGB555;
    case DRM_FORMAT_XBGR1555:
        break;
    case DRM_FORMAT_RGBX5551:
        break;
    case DRM_FORMAT_BGRX5551:
        break;
    case DRM_FORMAT_ARGB1555:
        break;
    case DRM_FORMAT_ABGR1555:
        break;
    case DRM_FORMAT_RGBA5551:
        break;
    case DRM_FORMAT_BGRA5551:
        break;
    case DRM_FORMAT_RGB565:
        return QImage::Format_RGB16;
    case DRM_FORMAT_BGR565:
        break;

    /* 24 bpp RGB */
    case DRM_FORMAT_RGB888:
        return QImage::Format_RGB888;
    case DRM_FORMAT_BGR888:
        return QImage::Format_BGR888;

    /* 32 bpp RGB */
    case DRM_FORMAT_XRGB8888:
        return QImage::Format_RGB32;
    case DRM_FORMAT_XBGR8888:
        break;
    case DRM_FORMAT_RGBX8888:
        return QImage::Format_RGBX8888;
    case DRM_FORMAT_BGRX8888:
        break;
    case DRM_FORMAT_ARGB8888:
        return QImage::Format_ARGB32_Premultiplied;
    case DRM_FORMAT_ABGR8888:
        break;
    case DRM_FORMAT_RGBA8888:
        return QImage::Format_RGBA8888_Premultiplied;
    case DRM_FORMAT_BGRA8888:
        break;
    case DRM_FORMAT_XRGB2101010:
        return QImage::Format_RGB30;
    case DRM_FORMAT_XBGR2101010:
        return QImage::Format_BGR30;
    case DRM_FORMAT_RGBX1010102:
        break;
    case DRM_FORMAT_BGRX1010102:
        break;
    case DRM_FORMAT_ARGB2101010:
        return QImage::Format_A2RGB30_Premultiplied;
    case DRM_FORMAT_ABGR2101010:
        return QImage::Format_A2BGR30_Premultiplied;
    case DRM_FORMAT_RGBA1010102:
        break;
    case DRM_FORMAT_BGRA1010102:
        break;
    }

    return QImage::Format_Invalid;
}

QW_BEGIN_NAMESPACE

struct _wlr_renderer : public wlr_renderer
{
    _wlr_renderer(QWRendererInterface *i)
        : interface(i)
    {}

    QWRendererInterface *interface;
};

inline static QWRendererInterface *interface(wlr_renderer *handle)
{
    auto _handle = static_cast<_wlr_renderer *>(handle);
    Q_ASSERT(_handle->interface && _handle->impl == handle->impl);
    return static_cast<_wlr_renderer *>(handle)->interface;
}

namespace impl {
static bool bind_buffer(wlr_renderer *handle, wlr_buffer *buffer)
{
    return interface(handle)->bindBuffer(QWBuffer::from(buffer));
}

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR <= 16
static void begin(wlr_renderer *handle, uint32_t width, uint32_t height)
#else
static bool begin(wlr_renderer *handle, uint32_t width, uint32_t height)
#endif
{
    return interface(handle)->begin({uint32ToPixelSize(width), uint32ToPixelSize(height)});
}

static void end(wlr_renderer *handle)
{
    return interface(handle)->end();
}

static void clear(wlr_renderer *handle, const float color[4])
{
    return interface(handle)->clear(QColor(color[0], color[1], color[2], color[3]));
}

static void scissor(wlr_renderer *handle, wlr_box *box)
{
    return interface(handle)->scissor({box->x, box->y, box->width, box->height});
}

static bool render_subtexture_with_matrix(wlr_renderer *handle,
                                          wlr_texture *texture,
                                          const wlr_fbox *box,
                                          const float matrix[9],
                                          float alpha)
{
    return interface(handle)->renderSubtextureWithMatrix(QWTexture::from(texture),
                                                         {box->x, box->y, box->width, box->height},
                                                         QMatrix3x3(matrix),
                                                         alpha);
}

static void render_quad_with_matrix(wlr_renderer *handle,
                                    const float color[4],
                                    const float matrix[9])
{
    return interface(handle)->renderQuadWithMatrix(QColor(color[0], color[1], color[2], color[3]),
                                                   QMatrix3x3(matrix));
}

static const uint32_t *get_shm_texture_formats(wlr_renderer *handle, size_t *len)
{
    auto formats = interface(handle)->getShmTextureFormats();
    *len = formats ? formats->size() : 0;
    return formats ? formats->data() : nullptr;
}

static const wlr_drm_format_set *get_dmabuf_texture_formats(wlr_renderer *handle)
{
    return interface(handle)->getDmabufTextureFormats();
}

static const wlr_drm_format_set *get_render_formats(wlr_renderer *handle)
{
    return interface(handle)->getRenderFormats();
}

static uint32_t preferred_read_format(wlr_renderer *handle)
{
    return interface(handle)->preferredReadFormat();
}

static bool read_pixels(wlr_renderer *handle,
                        uint32_t fmt,
                        uint32_t stride,
                        uint32_t width,
                        uint32_t height,
                        uint32_t srcX,
                        uint32_t srcY,
                        uint32_t dstX,
                        uint32_t dstY,
                        void *data)
{
    QImage::Format format {toQImageFormat(fmt)};
    if (format == QImage::Format::Format_Invalid) {
        qFatal("%s", QString("bad format. [format]: %1").arg(fmt).toUtf8().data());
    }

    auto pixelFormat {QImage::toPixelFormat(format)};
    auto pixelSize = pixelFormat.bitsPerPixel();
    QImage image {static_cast<uchar *>(data) + dstY * stride + dstX * pixelSize,
                 uint32ToPixelSize(width),
                 uint32ToPixelSize(height),
                 uint32ToPixelSize(stride),
                 format};

    const QRect srcRect {uint32ToPixelSize(srcX),
                        uint32ToPixelSize(srcY),
                        uint32ToPixelSize(width),
                        uint32ToPixelSize(height)};

    return interface(handle)->readPixels(srcRect, image);
}

static void destroy(wlr_renderer *handle)
{
    delete interface(handle);
}

static int get_drm_fd(wlr_renderer *handle)
{
    return interface(handle)->getDrmFd();
}

static uint32_t get_render_buffer_caps(wlr_renderer *handle)
{
    return interface(handle)->getRenderBufferCaps();
}

static wlr_texture *texture_from_buffer(wlr_renderer *handle, wlr_buffer *buffer)
{
    auto *texture = interface(handle)->textureFromBuffer(QWBuffer::from(buffer));
    return texture ? texture->handle() : nullptr;
}

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
static wlr_render_pass *begin_buffer_pass(wlr_renderer *handle, wlr_buffer *buffer, const wlr_buffer_pass_options *options) {
    return interface(handle)->beginBufferPass(QWBuffer::from(buffer), options);
}

static wlr_render_timer *render_timer_create(wlr_renderer *handle) {
    return interface(handle)->renderTimerCreate();
}
#endif
} // namespace impl

QWRendererInterface::~QWRendererInterface()
{
    free(handle());
    delete impl();
}

QWRendererInterface *QWRendererInterface::get(QWRenderer *handle)
{
    return interface(handle->handle());
}

bool QWRendererInterface::bindBuffer(QWBuffer *)
{
    return false;
}

const QVector<uint32_t>* QWRendererInterface::getShmTextureFormats() const
{
    return nullptr;
}

const wlr_drm_format_set *QWRendererInterface::getDmabufTextureFormats() const
{
    return nullptr;
}

const wlr_drm_format_set *QWRendererInterface::getRenderFormats() const
{
    return nullptr;
}

uint32_t QWRendererInterface::preferredReadFormat() const
{
    return 0;
}

bool QWRendererInterface::readPixels(const QRect &, QImage &)
{
    return false;
}

int QWRendererInterface::getDrmFd() const
{
    return 0;
}
uint32_t QWRendererInterface::getRenderBufferCaps() const
{
    return 0;
}

QWTexture *QWRendererInterface::textureFromBuffer(QWBuffer *) const
{
    return nullptr;
}

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
wlr_render_pass *QWRendererInterface::beginBufferPass(QWBuffer *, const wlr_buffer_pass_options *) {
    return nullptr;
}

wlr_render_timer *QWRendererInterface::renderTimerCreate() {
    return nullptr;
}
#endif

void QWRendererInterface::init(FuncMagicKey funMagicKey)
{
    auto impl = new wlr_renderer_impl {
        QW_INIT_INTERFACE_FUNC(funMagicKey, bind_buffer, &QWRendererInterface::bindBuffer),
        QW_INIT_INTERFACE_FUNC(funMagicKey, begin, &QWRendererInterface::begin),
        QW_INIT_INTERFACE_FUNC(funMagicKey, end, &QWRendererInterface::end),
        QW_INIT_INTERFACE_FUNC(funMagicKey, clear, &QWRendererInterface::clear),
        QW_INIT_INTERFACE_FUNC(funMagicKey, scissor, &QWRendererInterface::scissor),
        QW_INIT_INTERFACE_FUNC(funMagicKey, render_subtexture_with_matrix, &QWRendererInterface::renderSubtextureWithMatrix),
        QW_INIT_INTERFACE_FUNC(funMagicKey, render_quad_with_matrix, &QWRendererInterface::renderQuadWithMatrix),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_shm_texture_formats, &QWRendererInterface::getShmTextureFormats),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_dmabuf_texture_formats, &QWRendererInterface::getDmabufTextureFormats),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_render_formats, &QWRendererInterface::getRenderFormats),
        QW_INIT_INTERFACE_FUNC(funMagicKey, preferred_read_format, &QWRendererInterface::preferredReadFormat),
        QW_INIT_INTERFACE_FUNC(funMagicKey, read_pixels, &QWRendererInterface::readPixels),
        .destroy = impl::destroy,
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_drm_fd, &QWRendererInterface::getDrmFd),
        QW_INIT_INTERFACE_FUNC(funMagicKey, get_render_buffer_caps, &QWRendererInterface::getRenderBufferCaps),
        QW_INIT_INTERFACE_FUNC(funMagicKey, texture_from_buffer, &QWRendererInterface::textureFromBuffer),
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
        QW_INIT_INTERFACE_FUNC(funMagicKey, begin_buffer_pass, &QWRendererInterface::beginBufferPass),
        QW_INIT_INTERFACE_FUNC(funMagicKey, render_timer_create, &QWRendererInterface::renderTimerCreate),
#endif
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_renderer));
    static_cast<_wlr_renderer *>(m_handle)->interface = this;
    wlr_renderer_init(handle()->handle(), impl);
}

QW_END_NAMESPACE
