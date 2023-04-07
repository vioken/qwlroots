// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_backend;
struct wlr_backend_impl;

QW_BEGIN_NAMESPACE

class QW_EXPORT QWBackendInterface : public QWInterface
{
    friend class QWBackend;
public:
    virtual ~QWBackendInterface();
    virtual bool start() = 0;

    virtual clockid_t getPresentationClock() const;
    virtual int getDrmFd() const;
    virtual int getBufferCaps() const;

    inline wlr_backend *handle() const {
        return QWInterface::handle<wlr_backend>();
    }
    inline wlr_backend_impl *impl() const {
        return QWInterface::handle<wlr_backend_impl>();
    }
    static QWBackendInterface *get(wlr_backend *handle);

protected:
    template<class T>
    inline void init() {
        init(getFuncMagicKey<T>(&T::getPresentationClock, &T::getDrmFd, &T::getBufferCaps));
    }

    virtual void init(FuncMagicKey funMagicKey);
};

QW_END_NAMESPACE
