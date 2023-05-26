// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdamagering.h"

#include <QRect>

extern "C" {
#include <wlr/types/wlr_damage_ring.h>
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

wlr_damage_ring *QWDamageRing::handle() const
{
    return reinterpret_cast<wlr_damage_ring*>(const_cast<QWDamageRing*>(this));
}

QWDamageRing *QWDamageRing::from(wlr_damage_ring *handle)
{
    return reinterpret_cast<QWDamageRing*>(handle);
}

void QWDamageRing::init()
{
    wlr_damage_ring_init(handle());
}

void QWDamageRing::finish()
{
    wlr_damage_ring_finish(handle());
}

bool QWDamageRing::add(pixman_region32_t *damage)
{
    return wlr_damage_ring_add(handle(), damage);
}

bool QWDamageRing::addBox(const QRect &box)
{
    const wlr_box wbox {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };
    return wlr_damage_ring_add_box(handle(), &wbox);
}

void QWDamageRing::addWhole()
{
    wlr_damage_ring_add_whole(handle());
}

void QWDamageRing::ringRotate()
{
    wlr_damage_ring_rotate(handle());
}

void QWDamageRing::getBufferDamage(int bufferAge, pixman_region32_t *damage) const
{
    wlr_damage_ring_get_buffer_damage(handle(), bufferAge, damage);
}

QW_END_NAMESPACE
