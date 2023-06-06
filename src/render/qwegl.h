// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <EGL/egl.h>
}

struct wlr_egl;

QW_BEGIN_NAMESPACE

class QW_EXPORT QWEgl {
public:
    QWEgl() = delete;
    ~QWEgl() = delete;

    static QWEgl* createWithContext(EGLDisplay display, EGLContext context);
    static QWEgl* from(wlr_egl* wlr_egl);

    EGLDisplay getDisplay() const;
    EGLContext getContext() const;

    wlr_egl* handle() const;
};

QW_END_NAMESPACE
