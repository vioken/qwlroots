// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>
#include <QMatrix3x3>

#include <qwrendererinterface.h>

struct wlr_renderer;
struct wlr_box;
struct wlr_fbox;
struct wlr_drm_format_set;
struct wl_display;
struct wlr_render_texture_options;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWBackend;
class QWBuffer;
class QWTexture;
class QWRendererPrivate;
class QW_EXPORT QWRenderer : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWRenderer)
public:
    inline wlr_renderer *handle() const {
        return QWObject::handle<wlr_renderer>();
    }

    static QWRenderer *get(wlr_renderer *handle);
    static QWRenderer *from(wlr_renderer *handle);
    static QWRenderer *autoCreate(QWBackend *backend);
#if WLR_VERSION_MINOR > 16
    bool begin(uint32_t width, uint32_t height);
#else
    void begin(uint32_t width, uint32_t height);
#endif
#if WLR_VERSION_MINOR > 17
    // TODO: use QWRenderPass
    wlr_render_pass* begin(QWBuffer *buffer, const wlr_buffer_pass_options *options);
    void end(wlr_render_pass* pass);
#else
    bool begin(QWBuffer *buffer);
    void end();
#endif

    bool initWlDisplay(QWDisplay *display);
    bool initWlShm(QWDisplay *display);

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    void clear(const float *color);
    void clear(const QColor &color);
    void scissor(wlr_box *box);
    void scissor(const QRect &box);
    void renderTexture(QWTexture *texture, const float *projection, int x, int y, float alpha);
    void renderTexture(QWTexture *texture, const float *matrix, float alpha);
    void renderSubtexture(QWTexture *texture, wlr_fbox *fbox, const float *matrix, float alpha);
    void renderRect(const wlr_box *box, const float *color, const float *projection);
    void renderRect(const QRect &box, const QColor &color, const QMatrix3x3 &projection);
    void renderQuad(const float *color, const float *matrix);
    void renderQuad(const QColor &color, const QMatrix3x3 &matrix);
#endif

    const uint32_t *getShmTextureFormats(size_t *len) const;
    const wlr_drm_format_set *getDmabufTextureFormats() const;
#if WLR_VERSION_MINOR < 18
    bool readPixels(uint32_t fmt, uint32_t stride, uint32_t width, uint32_t height,
                    uint32_t src_x, uint32_t src_y, uint32_t dst_x, uint32_t dst_y, void *data) const;
#endif
    int getDrmFd() const;

    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWRendererInterface, Interface>::value, QWInterface*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWRendererInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWRenderer(i->handle(), true);
    }


Q_SIGNALS:
    void beforeDestroy(QWRenderer *self);
#if WLR_VERSION_MINOR > 16
    void lost();
#endif

private:
    QWRenderer(wlr_renderer *handle, bool isOwner);
    ~QWRenderer() = default;
};

QW_END_NAMESPACE
