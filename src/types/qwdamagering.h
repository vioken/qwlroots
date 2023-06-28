// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_damage_ring;
struct pixman_region32;
typedef pixman_region32 pixman_region32_t;

QT_BEGIN_NAMESPACE
class QRect;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QW_EXPORT QWDamageRing
{
public:
    QWDamageRing() = delete;
    ~QWDamageRing() = delete;

    wlr_damage_ring *handle() const;

    static QWDamageRing *from(wlr_damage_ring *handle);

    void init();
    void finish();
    bool add(pixman_region32_t *damage);
    bool addBox(const QRect &box);
    void addWhole();
    void ringRotate();
    void getBufferDamage(int bufferAge, pixman_region32_t *damage) const;
};

QW_END_NAMESPACE
