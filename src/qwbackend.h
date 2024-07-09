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

    QW_FUNC_STATIC(backend, autocreate, wlr_backend *, wl_display *display, wlr_session **session_ptr)

    QW_FUNC_STATIC(backend, is_multi, bool, wlr_backend *backend)
    QW_FUNC_STATIC(backend, is_drm, bool, wlr_backend *backend)
    QW_FUNC_STATIC(backend, is_wl, bool, wlr_backend *backend)
#ifdef WLR_HAVE_X11_BACKEND
    QW_FUNC_STATIC(backend, is_x11, bool, wlr_backend *backend)
#endif
    QW_FUNC_STATIC(backend, is_libinput, bool, wlr_backend *backend)
    QW_FUNC_STATIC(backend, is_headless, bool, wlr_backend *backend)

    QW_FUNC_MEMBER(backend, get_drm_fd, int)

protected:
    QW_FUNC_MEMBER(backend, destroy, void)
};

class qw_multi_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_MEMBER(multi_backend, add, bool, wlr_backend *backend)
    QW_FUNC_MEMBER(multi_backend, remove, void, wlr_backend *backend)
    QW_FUNC_MEMBER(multi, is_empty, bool)
    QW_FUNC_MEMBER(multi, for_each_backend, void, void (*callback)(wlr_backend *backend, void *data), void *data)

    QW_FUNC_STATIC(multi_backend, create, wlr_backend *, wl_display *display)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

class qw_drm_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(drm_backend, create, wlr_backend *, wl_display *display, wlr_session *session, wlr_device *dev, wlr_backend *parent)
    // TODO:move to qw_output
    // QW_FUNC_STATIC(output, is_drm)

    QW_FUNC_MEMBER(drm_backend, get_non_master_fd, int)

    QW_FUNC_STATIC(drm, connector_get_id, uint32_t, wlr_output *output)
    QW_FUNC_STATIC(drm, connector_add_mode, wlr_output_mode *, wlr_output *output, const drmModeModeInfo *mode)
    QW_FUNC_STATIC(drm, connector_get_panel_orientation, enum wl_output_transform, wlr_output *output)
    QW_FUNC_STATIC(drm, mode_get_info, const drmModeModeInfo *, wlr_output_mode *mode)
    QW_FUNC_STATIC(drm, create_lease, wlr_drm_lease *, wlr_output **outputs, size_t n_outputs, int *lease_fd)
    QW_FUNC_STATIC(drm, lease_terminate, void, wlr_drm_lease *lease)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

class qw_wayland_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_MEMBER(wl_backend, create, wlr_backend *, wl_display *remote_display)
    QW_FUNC_MEMBER(wl_backend, get_remote_display, wl_display *)
    QW_FUNC_MEMBER(wl, output_create, wlr_output *)
    QW_FUNC_MEMBER(wl, output_create_from_surface, wlr_output *, wl_surface *surface)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_wl)
    // QW_FUNC_MEMBER(output, is_wl)

    QW_FUNC_STATIC(wl, output_set_title, void, wlr_output *output, const char *title)
    QW_FUNC_STATIC(wl, output_get_surface, wl_surface *, wlr_output *output)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};

#ifdef WLR_HAVE_X11_BACKEND
class qw_x11_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(x11_backend, create, wlr_backend *, wl_display *display, const char *x11_display)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_x11)
    // QW_FUNC_MEMBER(output, is_x11)

    QW_FUNC_MEMBER(x11, output_create, wlr_output *)
    QW_FUNC_MEMBER(x11, output_set_title, void, const char *title)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;
};
#endif

class qw_libinput_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(libinput_backend, create, wlr_backend *, wl_display *display, wlr_session *session)
    QW_FUNC_STATIC(libinput, get_device_handle, libinput_device *, wlr_input_device *dev)

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
    QW_FUNC_STATIC(headless_backend, create, wlr_backend *, wl_display *display)

    QW_FUNC_MEMBER(headless, add_output, wlr_output *, unsigned int width, unsigned int height)

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
