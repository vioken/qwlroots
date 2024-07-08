// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/types/wlr_damage_ring.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(damage_ring)
{
public:
    // Empty parameters
    QW_FUNC_MEMBER(damage_ring, init)
    // Empty parameters
    QW_FUNC_MEMBER(damage_ring, finish)
    // const pixman_region32_t *damage
    QW_FUNC_MEMBER(damage_ring, add)
    // const wlr_box *box
    QW_FUNC_MEMBER(damage_ring, add_box)
    // Empty parameters
    QW_FUNC_MEMBER(damage_ring, add_whole)
    // Empty parameters
    QW_FUNC_MEMBER(damage_ring, rotate)
    // int buffer_age, pixman_region32_t *damage
    QW_FUNC_MEMBER(damage_ring, get_buffer_damage)
    // int32_t width, int32_t height
    QW_FUNC_MEMBER(damage_ring, set_bounds)
};

QW_END_NAMESPACE
