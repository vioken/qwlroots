// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_xcursor_manager;
struct wlr_xcursor;

QW_BEGIN_NAMESPACE

class QWCursor;
class QW_CLASS_REINTERPRET_CAST(xcursor_manage)
{
public:
    QW_FUNC_STATIC(xcursor_manage, create)

    QW_FUNC_MEMBER(xcursor_manage, destroy)
    QW_FUNC_MEMBER(xcursor_manage, load)
    QW_FUNC_MEMBER(xcursor_manage, get_xcursor)
};

QW_END_NAMESPACE
