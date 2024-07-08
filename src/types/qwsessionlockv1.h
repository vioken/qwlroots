// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(session_lock_surface_v1)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wlr_surface *surface
    QW_FUNC_STATIC(session_lock_surface_v1, try_from_wlr_surface)

    // uint32_t width, uint32_t height
    QW_FUNC_MEMBER(session_lock_surface_v1, configure)
};

class QW_CLASS_OBJECT(session_lock_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_surface, wlr_session_lock_surface_v1*)
    QW_SIGNAL(unlock)

public:
    // Empty parameters
    QW_FUNC_MEMBER(session_lock_v1, send_locked)
};

class QW_CLASS_OBJECT(session_lock_manager_v1)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(new_lock, wlr_session_lock_v1*)

public:
    // wl_display *display
    QW_FUNC_STATIC(session_lock_manager_v1, create)
};

QW_END_NAMESPACE
