// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_allocator;
struct wlr_drm_format;
struct wlr_buffer;

QW_BEGIN_NAMESPACE

class QWBuffer;
class QWBackend;
class QWRenderer;
class QWAllocatorPrivate;
class QW_EXPORT QWAllocator : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWAllocator)
public:
    inline wlr_allocator *handle() {
        return QWObject::handle<wlr_allocator>();
    }

    static QWAllocator *autoCreate(QWBackend *backend, QWRenderer *renderer);
    static QWAllocator *get(wlr_allocator *handle);
    static QWAllocator *from(wlr_allocator *handle);

    QWBuffer *createBuffer(int width, int height, const wlr_drm_format *format);

Q_SIGNALS:
    void beforeDestroy(QWAllocator *self);

private:
    QWAllocator(wlr_allocator *handle, bool isOwner);
    ~QWAllocator() = default;
};

QW_END_NAMESPACE
