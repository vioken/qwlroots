// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>
#include <QMatrix3x3>

struct wlr_renderer;
struct wlr_box;
struct wlr_fbox;
struct wlr_drm_format_set;
struct wl_display;

QW_BEGIN_NAMESPACE

class QWBackend;
class QWBuffer;
class QWTexture;
class QWRendererPrivate;
class QW_EXPORT QWRenderer : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWRenderer)
public:
    explicit QWRenderer(wlr_renderer *handle, QObject *parent = nullptr);
    ~QWRenderer();

    static QWRenderer *autoCreate(QWBackend *backend);

    inline wlr_renderer *handle() const {
        return QWObject::handle<wlr_renderer>();
    }

    void begin(uint32_t width, uint32_t height);
    void begin(QWBuffer *buffer);
    void end();

    bool initWlDisplay(wl_display *display);
    bool initWlShm(wl_display *display);

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

    const uint32_t *getShmTextureFormats(size_t *len) const;
    const wlr_drm_format_set *getDmabufTextureFormats() const;
    bool readPixels(uint32_t fmt, uint32_t stride, uint32_t width, uint32_t height,
                    uint32_t src_x, uint32_t src_y, uint32_t dst_x, uint32_t dst_y, void *data) const;
    int getDrmFd() const;
};

QW_END_NAMESPACE
