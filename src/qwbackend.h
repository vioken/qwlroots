// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

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
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(backend)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_output, wlr_output*)
    QW_SIGNAL(new_input, wlr_input_device*)

public:
    static inline DeriveType *create(HandleType *handle);

    // wl_display *display, wlr_session **session_ptr
    QW_FUNC_STATIC(backend, autocreate)

    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_multi)
    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_drm)
    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_wl)
#ifdef WLR_HAVE_X11_BACKEND
    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_x11)
#endif
    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_libinput)
    // wlr_backend *backend
    QW_FUNC_STATIC(backend, is_headless)

    // Empty parameters
    QW_FUNC_MEMBER(backend, get_drm_fd)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(backend, destroy)
};

class qw_multi_backend : public qw_backend
{
    Q_OBJECT

public:
    // wlr_backend *backend
    QW_FUNC_MEMBER(multi_backend, add)
    // wlr_backend *backend
    QW_FUNC_MEMBER(multi_backend, remove)
    // Empty parameters
    QW_FUNC_MEMBER(multi, is_empty)
    // wlr_backend *backend, void (*callback)(wlr_backend *backend, void *data), void *data
    QW_FUNC_MEMBER(multi, for_each_backend)

    // wl_display *display
    QW_FUNC_STATIC(multi_backend, create)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

class qw_drm_backend : public qw_backend
{
    Q_OBJECT

public:
    // wl_display *display, wlr_session *session, wlr_device *dev, wlr_backend *parent
    QW_FUNC_STATIC(drm_backend, create)
    // TODO:move to qw_output
    // QW_FUNC_STATIC(output, is_drm)

    // Empty parameters
    QW_FUNC_MEMBER(drm_backend, get_non_master_fd)

    // wlr_output *output
    QW_FUNC_STATIC(drm, connector_get_id)
    // wlr_output *output, const drmModeModeInfo *mode
    QW_FUNC_STATIC(drm, connector_add_mode)
    // wlr_output *output
    QW_FUNC_STATIC(drm, connector_get_panel_orientation)
    // wlr_output_mode *mode
    QW_FUNC_STATIC(drm, mode_get_info)
    // wlr_output **outputs, size_t n_outputs, int *lease_fd
    QW_FUNC_STATIC(drm, create_lease)
    // wlr_drm_lease *lease
    QW_FUNC_STATIC(drm, lease_terminate)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

class qw_wayland_backend : public qw_backend
{
    Q_OBJECT

public:
    // wl_display *remote_display
    QW_FUNC_MEMBER(wl_backend, create)
    // Empty parameters
    QW_FUNC_MEMBER(wl_backend, get_remote_display)
    // Empty parameters
    QW_FUNC_MEMBER(wl, output_create)
    // wl_surface *surface
    QW_FUNC_MEMBER(wl, output_create_from_surface)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_wl)
    // QW_FUNC_MEMBER(output, is_wl)

    // wlr_output *output, const char *title
    QW_FUNC_STATIC(wl, output_set_title)
    // wlr_output *output
    QW_FUNC_STATIC(wl, output_get_surface)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

#ifdef WLR_HAVE_X11_BACKEND
class qw_x11_backend : public qw_backend
{
    Q_OBJECT

public:
    // wl_display *display, const char *x11_display
    QW_FUNC_STATIC(x11_backend, create)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_x11)
    // QW_FUNC_MEMBER(output, is_x11)

    // Empty parameters
    QW_FUNC_MEMBER(x11, output_create)
    // const char *title
    QW_FUNC_MEMBER(x11, output_set_title)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};
#endif

class qw_libinput_backend : public qw_backend
{
    Q_OBJECT

public:
    // wl_display *display, wlr_session *session
    QW_FUNC_STATIC(libinput_backend, create)
    // wlr_input_device *dev
    QW_FUNC_STATIC(libinput, get_device_handle)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_libinput)
    // QW_FUNC_MEMBER(output, is_libinput)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

class qw_headless_backend : public qw_backend
{
    Q_OBJECT

public:
    // wl_display *display
    QW_FUNC_STATIC(headless_backend, create)

    // unsigned int width, unsigned int height
    QW_FUNC_MEMBER(headless, add_output)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_headless)
    // QW_FUNC_MEMBER(output, is_headless)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

qw_backend *qw_backend::create(HandleType *handle) {
    if (is_multi(handle))
        return new qw_multi_backend(handle, false);
#ifdef WLR_HAVE_X11_BACKEND
    if (is_x11(handle))
        return new qw_x11_backend(handle, false);
#endif
    if (is_drm(handle))
        return new qw_drm_backend(handle, false);
    if (is_headless(handle))
        return new qw_headless_backend(handle, false);
    if (is_libinput(handle))
        return new qw_libinput_backend(handle, false);
    if (is_wl(handle))
        return new qw_wayland_backend(handle, false);

    return new DeriveType(handle, false);
}

QW_END_NAMESPACE
