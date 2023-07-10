// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>
#include <QPoint>

struct wlr_output;
struct wlr_output_impl;
struct wlr_output_state;
struct wlr_drm_format_set;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWBackend;
class QWBuffer;
class QWOutput;
class QW_EXPORT QWOutputInterface : public QWInterface
{
    friend class QWOutput;
public:
    virtual ~QWOutputInterface();
    virtual bool setCursor(QWBuffer *buffer, const QPoint &hotspot);
    virtual bool moveCursor(const QPoint &pos);
    virtual bool test(const wlr_output_state *state) const;
    virtual bool commit(const wlr_output_state *state) = 0;

    virtual size_t getGammaSize() const;
    virtual const wlr_drm_format_set *getCursorFormats(uint32_t bufferCaps) const;
    virtual const wlr_drm_format_set *getPrimaryFormats(uint32_t bufferCaps) const;
    virtual QPoint getCursorSize() const;

    inline wlr_output *handle() const {
        return QWInterface::handle<wlr_output>();
    }
    inline wlr_output_impl *impl() const {
        return QWInterface::handle<wlr_output_impl>();
    }
    static QWOutputInterface *get(wlr_output *handle);

protected:
    template<class T>
    inline void init(QWBackend *backend, QWDisplay *display) {
        init(getFuncMagicKey<T>(&T::setCursor, &T::moveCursor, &T::test,
                                &T::getGammaSize, &T::getCursorFormats,
                                &T::getPrimaryFormats, &T::getCursorSize),
             backend, display);
    }

    virtual void init(FuncMagicKey funMagicKey, QWBackend *backend, QWDisplay *display);
};

QW_END_NAMESPACE
