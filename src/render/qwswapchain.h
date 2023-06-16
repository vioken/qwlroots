// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_swapchain;
struct wlr_drm_format;
class QSize;

QW_BEGIN_NAMESPACE

class QWAllocator;
class QWBuffer;
class QW_EXPORT QWSwapchain {
public:
    QWSwapchain() = delete;
    ~QWSwapchain() = delete;

    void operator delete(QWSwapchain *p, std::destroying_delete_t);

    static QWSwapchain *create(QWAllocator *alloc, QSize size, const wlr_drm_format *format);
    static QWSwapchain *from(wlr_swapchain* swapchain);

    wlr_swapchain* handle() const;

    QWBuffer *acquire(int *age);
    void setBufferSubmitted(QWBuffer *buffer);
};

QW_END_NAMESPACE
