// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <memory>
#include <qwglobal.h>

struct wlr_damage_ring;
struct pixman_region32;
typedef pixman_region32 pixman_region32_t;

QT_BEGIN_NAMESPACE
class QRect;
class QSize;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QW_EXPORT QWDamageRing
{
public:
    explicit QWDamageRing();
    ~QWDamageRing();

    wlr_damage_ring *handle() const;

    bool add(pixman_region32_t *damage);
    bool addBox(const QRect &box);
    void addWhole();
    void rotate();
    void getBufferDamage(int bufferAge, pixman_region32_t *damage) const;
    void setBounds(const QSize &bounds);

private:
    std::unique_ptr<wlr_damage_ring> m_handle;
};

QW_END_NAMESPACE
