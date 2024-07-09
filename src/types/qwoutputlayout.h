// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_layout.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(output_layout)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(add, wlr_output_layout_output*)
    QW_SIGNAL(change, wlr_output_layout_output*)

public:
    QW_FUNC_STATIC(output_layout, create)

    QW_FUNC_MEMBER(output_layout, get)
    QW_FUNC_MEMBER(output_layout, output_at)
    QW_FUNC_MEMBER(output_layout, get_center_output)
    QW_FUNC_MEMBER(output_layout, adjacent_output)
    QW_FUNC_MEMBER(output_layout, farthest_output)

    QW_FUNC_MEMBER(output_layout, add)
    QW_FUNC_MEMBER(output_layout, add_auto)
    QW_FUNC_MEMBER(output_layout, remove)

    QW_FUNC_MEMBER(output_layout, output_coords)
    QW_FUNC_MEMBER(output_layout, contains_point)
    QW_FUNC_MEMBER(output_layout, intersects)
    QW_FUNC_MEMBER(output_layout, closest_point)
    QW_FUNC_MEMBER(output_layout, get_box)

protected:
    QW_FUNC_MEMBER(output_layout, destroy)
};

QW_END_NAMESPACE
