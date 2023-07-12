// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

#include <QGenericMatrix>

struct wlr_renderer_impl;
struct wlr_drm_format_set;
struct wlr_render_pass;
struct wlr_buffer_pass_options;
struct wlr_render_timer;

class QImage;
class QPoint;

QW_BEGIN_NAMESPACE
class QWTexture;
class QWBuffer;
class QWRenderer;
class QWRendererInterface : public QWInterface
{
    friend class QWRenderer;
public:
    virtual ~QWRendererInterface();

    virtual bool bindBuffer(QWBuffer *buffer);
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR <= 16
    virtual void begin(const QSize &size) = 0;
#else
    virtual bool begin(const QSize &size) = 0;
#endif
    virtual void end() = 0;
    virtual void clear(const QColor &color) = 0;
    virtual void scissor(const QRect &rect) = 0;

    virtual bool renderSubtextureWithMatrix(QWTexture *texture,
                                            const QRectF &rect,
                                            const QMatrix3x3 &matrix,
                                            float alpha) = 0;

    virtual void renderQuadWithMatrix(const QColor &color, const QMatrix3x3 &matrix) = 0;

    virtual bool readPixels(const QRect &src, QImage &dstImage);
    virtual int getDrmFd() const;

    virtual uint32_t preferredReadFormat() const;
    virtual uint32_t getRenderBufferCaps() const;

    virtual QWTexture *textureFromBuffer(QWBuffer *buffer) const;

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
    wlr_render_pass *beginBufferPass(QWBuffer *buffer, const wlr_buffer_pass_options *options);
    wlr_render_timer *renderTimerCreate();
#endif

    virtual const QVector<uint32_t>* getShmTextureFormats() const;

    virtual const wlr_drm_format_set *getDmabufTextureFormats() const;
    virtual const wlr_drm_format_set *getRenderFormats() const;

    inline QWRenderer *handle() const { return QWInterface::handle<QWRenderer>(); }

    inline wlr_renderer_impl *impl() const { return QWInterface::handle<wlr_renderer_impl>(); }

    static QWRendererInterface *get(QWRenderer *handle);

protected:
    template<class T>
    inline void init() {
        init(getFuncMagicKey<T>(&T::bindBuffer,
                                &T::begin,
                                &T::end,
                                &T::clear,
                                &T::scissor,
                                &T::renderSubtextureWithMatrix,
                                &T::renderQuadWithMatrix,
                                &T::getShmTextureFormats,
                                &T::getDmabufTextureFormats,
                                &T::getRenderFormats,
                                &T::preferredReadFormat,
                                &T::readPixels,
                                &T::getDrmFd,
                                &T::getRenderBufferCaps,
                                &T::textureFromBuffer
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR > 16
                                ,
                                &T::beginBufferPass,
                                &T::renderTimerCreate
#endif
        ));
    }

    virtual void init(FuncMagicKey funMagicKey);
};

QW_END_NAMESPACE
