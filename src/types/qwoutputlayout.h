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
    // void
    QW_FUNC_STATIC(output_layout, create)

    // wlr_output *reference
    QW_FUNC_MEMBER(output_layout, get)
    // double lx, double ly
    QW_FUNC_MEMBER(output_layout, output_at)
    // Empty parameters
    QW_FUNC_MEMBER(output_layout, get_center_output)
    // enum wlr_direction direction, wlr_output *reference, double ref_lx, double ref_ly
    QW_FUNC_MEMBER(output_layout, adjacent_output)
    // enum wlr_direction direction, wlr_output *reference, double ref_lx, double ref_ly
    QW_FUNC_MEMBER(output_layout, farthest_output)

    // wlr_output *output, int lx, int ly
    QW_FUNC_MEMBER(output_layout, add)
    // wlr_output *output
    QW_FUNC_MEMBER(output_layout, add_auto)
    // wlr_output *output
    QW_FUNC_MEMBER(output_layout, remove)

    // wlr_output *reference, double *lx, double *ly
    QW_FUNC_MEMBER(output_layout, output_coords)
    // wlr_output *reference, int lx, int ly
    QW_FUNC_MEMBER(output_layout, contains_point)
    // wlr_output *reference, const wlr_box *target_lbox
    QW_FUNC_MEMBER(output_layout, intersects)
    // wlr_output *reference, double lx, double ly, double *dest_lx, double *dest_ly
    QW_FUNC_MEMBER(output_layout, closest_point)
    // wlr_output *reference, wlr_box *dest_box
    QW_FUNC_MEMBER(output_layout, get_box)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(output_layout, destroy)
};

QW_END_NAMESPACE
