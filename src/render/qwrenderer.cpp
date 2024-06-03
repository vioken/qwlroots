// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
// avoid replace static
#include <wayland-server-core.h>
#define static
#include <wlr/render/wlr_renderer.h>
#if WLR_VERSION_MINOR >= 18
#include <wlr/render/pass.h>
#endif
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWRendererPrivate : public QWWrapObjectPrivate
{
public:
    QWRendererPrivate(wlr_renderer *handle, bool isOwner, QWRenderer *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_renderer_destroy))
    {
        sc.connect(&handle->events.lost, this, &QWRendererPrivate::on_lost);
    }

    void on_lost(void *);

    QW_DECLARE_PUBLIC(QWRenderer)
};

void QWRendererPrivate::on_lost(void *)
{
    Q_EMIT q_func()->lost();
}

QWRenderer *QWRenderer::get(wlr_renderer *handle)
{
    return static_cast<QWRenderer*>(QWRendererPrivate::map.value(handle));
}

QWRenderer *QWRenderer::from(wlr_renderer *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWRenderer(handle, false);
}

QWRenderer *QWRenderer::autoCreate(QWBackend *backend)
{
    auto handle = wlr_renderer_autocreate(backend->handle());
    if (!handle)
        return nullptr;
    return new QWRenderer(handle, true);
}

QWRenderer::QWRenderer(wlr_renderer *handle, bool isOwner)
    : QWWrapObject(*new QWRendererPrivate(handle, isOwner, this))
{

}

#if WLR_VERSION_MINOR < 18
bool QWRenderer::begin(uint32_t width, uint32_t height)
{
    return wlr_renderer_begin(handle(), width, height);
}
#endif

#if WLR_VERSION_MINOR < 18
bool QWRenderer::begin(QWBuffer *buffer)
{
    return wlr_renderer_begin_with_buffer(handle(), buffer->handle());
}
# else
wlr_render_pass* QWRenderer::begin(QWBuffer *buffer, const wlr_buffer_pass_options *options)
{
    return wlr_renderer_begin_buffer_pass(handle(), buffer->handle(), options);
}
#endif
#if WLR_VERSION_MINOR > 17
void QWRenderer::end(wlr_render_pass *pass)
{
    wlr_render_pass_submit(pass);
}
#else
void QWRenderer::end()
{
    wlr_renderer_end(handle());
}
#endif

bool QWRenderer::initWlDisplay(QWDisplay *display)
{
    return wlr_renderer_init_wl_display(handle(), display->handle());
}

bool QWRenderer::initWlShm(QWDisplay *display)
{
    return wlr_renderer_init_wl_shm(handle(), display->handle());
}

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
void QWRenderer::clear(const float *color)
{
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
    wlr_render_texture(handle(), texture->handle(), projection, x, y, alpha);
}

void QWRenderer::renderTexture(QWTexture *texture, const float *matrix, float alpha)
{
    wlr_render_texture_with_matrix(handle(), texture->handle(), matrix, alpha);
}

void QWRenderer::renderSubtexture(QWTexture *texture, wlr_fbox *fbox, const float *matrix, float alpha)
{
    wlr_render_subtexture_with_matrix(handle(), texture->handle(), fbox, matrix, alpha);
}

void QWRenderer::renderRect(const wlr_box *box, const float *color, const float *projection)
{
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
#endif

const uint32_t *QWRenderer::getShmTextureFormats(size_t *len) const
{
    return wlr_renderer_get_shm_texture_formats(handle(), len);
}

const wlr_drm_format_set *QWRenderer::getDmabufTextureFormats() const
{
    return wlr_renderer_get_dmabuf_texture_formats(handle());
}

#if WLR_VERSION_MINOR < 18
bool QWRenderer::readPixels(uint32_t fmt, uint32_t stride, uint32_t width, uint32_t height,
                            uint32_t src_x, uint32_t src_y, uint32_t dst_x, uint32_t dst_y, void *data) const
{
    return wlr_renderer_read_pixels(handle(), fmt, stride, width, height, src_x, src_y, dst_x, dst_y, data);
}
#endif

int QWRenderer::getDrmFd() const
{
    return wlr_renderer_get_drm_fd(handle());
}

QW_END_NAMESPACE
