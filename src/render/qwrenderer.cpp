// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrenderer.h"
#include "qwbackend.h"
#include "qwtexture.h"
#include "types/qwbuffer.h"

#include <QColor>
#include <QRect>

extern "C" {
#define static
#include <wlr/render/wlr_renderer.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWRendererPrivate : public QWObjectPrivate
{
public:
    QWRendererPrivate(void *handle, QWRenderer *qq)
        : QWObjectPrivate(handle, qq)
    {

    }
    ~QWRendererPrivate() {
        // m_handle destory follow the wlr_backend
    }

    QW_DECLARE_PUBLIC(QWRenderer)
};

QWRenderer *QWRenderer::autoCreate(QWBackend *backend)
{
    auto handle = wlr_renderer_autocreate(backend->handle());
    if (!handle)
        return nullptr;
    return new QWRenderer(handle, backend);
}

QWRenderer::QWRenderer(wlr_renderer *handle, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWRendererPrivate(handle, this))
{

}

QWRenderer::~QWRenderer()
{

}

void QWRenderer::begin(uint32_t width, uint32_t height)
{
    Q_D(QWRenderer);
    wlr_renderer_begin(handle(), width, height);
}

void QWRenderer::begin(QWBuffer *buffer)
{
    Q_D(QWRenderer);
    wlr_renderer_begin_with_buffer(handle(), buffer->handle());
}

void QWRenderer::end()
{
    Q_D(QWRenderer);
    wlr_renderer_end(handle());
}

bool QWRenderer::initWlDisplay(wl_display *display)
{
    Q_D(QWRenderer);
    return wlr_renderer_init_wl_display(handle(), display);
}

bool QWRenderer::initWlShm(wl_display *display)
{
    Q_D(QWRenderer);
    return wlr_renderer_init_wl_shm(handle(), display);
}

void QWRenderer::clear(const float *color)
{
    Q_D(QWRenderer);
    wlr_renderer_clear(handle(), color);
}

void QWRenderer::clear(const QColor &color)
{
    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };
    clear(c);
}

void QWRenderer::scissor(wlr_box *box)
{
    Q_D(QWRenderer);
    wlr_renderer_scissor(handle(), box);
}

void QWRenderer::scissor(const QRect &box)
{
    wlr_box b {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };

    scissor(&b);
}

void QWRenderer::renderTexture(QWTexture *texture, const float *projection, int x, int y, float alpha)
{
    Q_D(QWRenderer);
    wlr_render_texture(handle(), texture->handle(), projection, x, y, alpha);
}

void QWRenderer::renderTexture(QWTexture *texture, const float *matrix, float alpha)
{
    Q_D(QWRenderer);
    wlr_render_texture_with_matrix(handle(), texture->handle(), matrix, alpha);
}

void QWRenderer::renderSubtexture(QWTexture *texture, wlr_fbox *fbox, const float *matrix, float alpha)
{
    Q_D(QWRenderer);
    wlr_render_subtexture_with_matrix(handle(), texture->handle(), fbox, matrix, alpha);
}

void QWRenderer::renderRect(const wlr_box *box, const float *color, const float *projection)
{
    Q_D(QWRenderer);
    wlr_render_rect(handle(), box, color, projection);
}

void QWRenderer::renderRect(const QRect &box, const QColor &color, const QMatrix3x3 &projection)
{
    wlr_box b {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };

    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };

    renderRect(&b, c, projection.constData());
}

void QWRenderer::renderQuad(const float *color, const float *matrix)
{
    Q_D(QWRenderer);
    wlr_render_quad_with_matrix(handle(), color, matrix);
}

void QWRenderer::renderQuad(const QColor &color, const QMatrix3x3 &matrix)
{
    float c[4] {
        static_cast<float>(color.redF()),
        static_cast<float>(color.greenF()),
        static_cast<float>(color.blueF()),
        static_cast<float>(color.alphaF())
    };

    renderQuad(c, matrix.constData());
}

const uint32_t *QWRenderer::getShmTextureFormats(size_t *len) const
{
    Q_D(const QWRenderer);
    return wlr_renderer_get_shm_texture_formats(handle(), len);
}

const wlr_drm_format_set *QWRenderer::getDmabufTextureFormats() const
{
    Q_D(const QWRenderer);
    return wlr_renderer_get_dmabuf_texture_formats(handle());
}

bool QWRenderer::readPixels(uint32_t fmt, uint32_t stride, uint32_t width, uint32_t height,
                            uint32_t src_x, uint32_t src_y, uint32_t dst_x, uint32_t dst_y, void *data) const
{
    Q_D(const QWRenderer);
    return wlr_renderer_read_pixels(handle(), fmt, stride, width, height, src_x, src_y, dst_x, dst_y, data);
}

int QWRenderer::getDrmFd() const
{
    Q_D(const QWRenderer);
    return wlr_renderer_get_drm_fd(handle());
}

QW_END_NAMESPACE
