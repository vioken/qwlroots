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

public:
    static DeriveType *create(HandleType *handle) {
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

    QW_FUNC_MEMBER(backend, auto_create)
    QW_FUNC_MEMBER(backend, get_drm_fd)

    QW_FUNC_STATIC(backend, is_multi)
    QW_FUNC_STATIC(backend, is_drm)
    QW_FUNC_STATIC(backend, is_wl)
    QW_FUNC_STATIC(backend, is_x11)
    QW_FUNC_STATIC(backend, is_libinput)
    QW_FUNC_STATIC(backend, is_headless)

    QW_SIGNAL(new_output, wlr_output*)
    QW_SIGNAL(new_input, wlr_input_device*)

protected:
    QW_FUNC_MEMBER(backend, destroy)
};

class qw_multi_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_MEMBER(multi_backend, add)
    QW_FUNC_MEMBER(multi_backend, remove)
    QW_FUNC_MEMBER(multi_backend, is_empty)
    QW_FUNC_MEMBER(multi_backend, for_each_backend)

    QW_FUNC_STATIC(multi_backend, create)

protected:
    using qw_backend::qw_backend;
    friend class qw_backend;

};

class qw_drm_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(drm_backend, create)
    // TODO:move to qw_output
    // QW_FUNC_STATIC(output, is_drm)

    QW_FUNC_MEMBER(drm_backend, get_drm_fd)
    QW_FUNC_MEMBER(drm_backend, get_renderer)
    QW_FUNC_MEMBER(drm_backend, get_non_master_fd)

    QW_FUNC_STATIC(drm, connector_get_id)
    QW_FUNC_STATIC(drm, connector_add_mode)
    QW_FUNC_STATIC(drm, connector_get_panel_orientation)
    QW_FUNC_STATIC(drm, mode_get_info)
    QW_FUNC_STATIC(drm, create_lease)
    QW_FUNC_STATIC(drm, lease_terminate)
    QW_FUNC_STATIC(drm, lease_terminate)
};

class qw_wayland_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_MEMBER(wl_backend, create)
    QW_FUNC_MEMBER(wl_backend, get_remote_display)
    QW_FUNC_MEMBER(wl, output_create)
    QW_FUNC_MEMBER(wl, output_create_from_surface)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_wl)
    // QW_FUNC_MEMBER(output, is_wl)

    QW_FUNC_STATIC(wl, output_set_title)
    QW_FUNC_STATIC(wl, output_get_surface)
};

#ifdef WLR_HAVE_X11_BACKEND
class qw_x11_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(x11_backend, create)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_x11)
    // QW_FUNC_MEMBER(output, is_x11)

    QW_FUNC_MEMBER(x11, output_create)
    QW_FUNC_MEMBER(x11, output_set_title)
};
#endif

class qw_libinput_backend : public qw_backend
{
    Q_OBJECT

public:
    QW_FUNC_STATIC(libinput_backend, create)
    QW_FUNC_STATIC(libinput, get_device_handle)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_libinput)
    // QW_FUNC_MEMBER(output, is_libinput)
};

class qw_headless_backend : public qw_backend
{
    Q_OBJECT
public:
    QW_FUNC_STATIC(headless_backend, create)
    QW_FUNC_MEMBER(headless, add_output)

    // TODO:move to qw_output qw_input_device
    // QW_FUNC_MEMBER(input_device, is_headless)
    // QW_FUNC_MEMBER(output, is_headless)
};

QW_END_NAMESPACE
