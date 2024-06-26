// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

#include <functional>
#include <type_traits>
#include <utility>

extern "C" {
#include <wlr/backend.h>
#include <wlr/backend/multi.h>
#define static
#include <wlr/backend/drm.h>
#undef static
#include <wlr/backend/wayland.h>
#ifdef WLR_HAVE_X11_BACKEND
#include <wlr/backend/x11.h>
#endif
#include <wlr/backend/libinput.h>
#include <wlr/backend/headless.h>
#include <wlr/backend/interface.h>
}

struct wlr_backend;
struct wlr_backend_impl;

QW_BEGIN_NAMESPACE

class QW_EXPORT QWBackendInterface : public QWInterface
{
    friend class QWBackend;
public:
    virtual ~QWBackendInterface();
    virtual bool start() = 0;

    virtual int getDrmFd() const;
    virtual int getBufferCaps() const;

    inline wlr_backend *handle() const {
        return QWInterface::handle<wlr_backend>();
    }
    inline wlr_backend_impl *impl() const {
        return QWInterface::impl<wlr_backend_impl>();
    }
    static QWBackendInterface *get(wlr_backend *handle);

protected:
    template<class T>
    inline void init() {
        init(getFuncMagicKey<T>(&T::getPresentationClock, &T::getDrmFd, &T::getBufferCaps));
    }

    virtual void init(FuncMagicKey funMagicKey);
};

template<typename Derive>
class QW_CLASS_INTERFACE(backend)
{
public:
    ~qw_backend_interface() override {
        wlr_backend_finish(handle());
    }

    QW_INTERFACE(start)
    QW_INTERFACE(get_drm_fd)
    QW_INTERFACE(get_buffer_caps)
};

QW_END_NAMESPACE
