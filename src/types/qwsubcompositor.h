// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wl_display;
struct wlr_subcompositor;
struct wlr_subsurface;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSubcompositorPrivate;
class QW_EXPORT QWSubcompositor : public QWObject
{
    QW_DECLARE_PRIVATE(QWSubcompositor)
public:
    explicit QWSubcompositor(wlr_subcompositor *handle);

    static QWSubcompositor *create(QWDisplay *display);

    inline wlr_subcompositor *handle() const {
        return QWObject::handle<wlr_subcompositor>();
    }

private:
    ~QWSubcompositor() = default;
};

QW_END_NAMESPACE
