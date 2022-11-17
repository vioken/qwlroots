// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_allocator;
struct wlr_drm_format;
struct wlr_buffer;

QW_BEGIN_NAMESPACE

class QWBackend;
class QWRenderer;
class QWAllocatorPrivate;
class QW_EXPORT QWAllocator : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWAllocator)
public:
    explicit QWAllocator(wlr_allocator *handle);

    inline wlr_allocator *handle() {
        return QWObject::handle<wlr_allocator>();
    }

    static QWAllocator *autoCreate(QWBackend *backend, QWRenderer *renderer);

    wlr_buffer *createBuffer(int width, int height, const wlr_drm_format *format);
};

QW_END_NAMESPACE
