// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_shm;
struct wl_display;

QW_BEGIN_NAMESPACE

class QWRenderer;
class QW_EXPORT QWShm
{
public:
    QWShm() = delete;
    ~QWShm() = delete;

    wlr_shm *handle() const;

    static QWShm *from(wlr_shm *handle);
    static QWShm *create(wl_display *display, uint32_t version, const uint32_t *formats, size_t formatsLen);
    static QWShm *create(wl_display *display, uint32_t version, QWRenderer *renderer);
};

QW_END_NAMESPACE
