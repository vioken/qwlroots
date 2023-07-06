// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrenderer.h"
#include "qwbackend.h"
#include "qwdisplay.h"
#include "qwtexture.h"
#include "types/qwbuffer.h"
#include "util/qwsignalconnector.h"

#include <QColor>
#include <QRect>
#include <QHash>

extern "C" {
// avoid replace static
#include <wayland-server-core.h>
#define static
#include <wlr/render/wlr_renderer.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWRendererPrivate : public QWObjectPrivate
{
public:
    QWRendererPrivate(wlr_renderer *handle, bool isOwner, QWRenderer *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWRendererPrivate::on_destroy);
    }
    ~QWRendererPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_renderer_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWRenderer*> map;
    QW_DECLARE_PUBLIC(QWRenderer)
    QWSignalConnector sc;
};
QHash<void*, QWRenderer*> QWRendererPrivate::map;

void QWRendererPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWRenderer *QWRenderer::get(wlr_renderer *handle)
{
    return QWRendererPrivate::map.value(handle);
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
    : QObject(nullptr)
    , QWObject(*new QWRendererPrivate(handle, isOwner, this))
{

}

#if WLR_VERSION_MINOR > 16
bool QWRenderer::begin(uint32_t width, uint32_t height)
{
    Q_D(QWRenderer);
    return wlr_renderer_begin(handle(), width, height);
}
#else
void QWRenderer::begin(uint32_t width, uint32_t height)
{
    Q_D(QWRenderer);
    wlr_renderer_begin(handle(), width, height);
}
#endif

bool QWRenderer::begin(QWBuffer *buffer)
{
    Q_D(QWRenderer);
    return wlr_renderer_begin_with_buffer(handle(), buffer->handle());
}

void QWRenderer::end()
{
    Q_D(QWRenderer);
    wlr_renderer_end(handle());
}

bool QWRenderer::initWlDisplay(QWDisplay *display)
{
    Q_D(QWRenderer);
    return wlr_renderer_init_wl_display(handle(), display->handle());
}

bool QWRenderer::initWlShm(QWDisplay *display)
{
    Q_D(QWRenderer);
    return wlr_renderer_init_wl_shm(handle(), display->handle());
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
