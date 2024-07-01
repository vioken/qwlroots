// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/types/wlr_damage_ring.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_REINTERPRET_CAST(damage_ring) {
public:
    QW_FUNC_MEMBER(damage_ring, init)
    QW_FUNC_MEMBER(damage_ring, finish)
    QW_FUNC_MEMBER(damage_ring, add)
    QW_FUNC_MEMBER(damage_ring, add_box)
    QW_FUNC_MEMBER(damage_ring, add_whole)
    QW_FUNC_MEMBER(damage_ring, rotate)
    QW_FUNC_MEMBER(damage_ring, get_buffer_damage)
    QW_FUNC_MEMBER(damage_ring, set_bounds)
};

QW_END_NAMESPACE
