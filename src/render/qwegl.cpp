// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwegl.h"

extern "C" {
#include <wlr/render/egl.h>
}

QW_BEGIN_NAMESPACE

QWEgl *QWEgl::createWithContext(EGLDisplay display, EGLContext context)
{
    return from(wlr_egl_create_with_context(display, context));
}

EGLDisplay QWEgl::getDisplay() const
{
    return wlr_egl_get_display(handle());
}

EGLContext QWEgl::getContext() const
{
    return wlr_egl_get_context(handle());
}

QWEgl* QWEgl::from(wlr_egl* wlr_egl) {
    return reinterpret_cast<QWEgl*>(wlr_egl);
}

wlr_egl* QWEgl::handle() const {
    return reinterpret_cast<wlr_egl*>(const_cast<QWEgl*>(this));
}

QW_END_NAMESPACE
