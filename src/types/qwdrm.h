// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_drm;
struct wlr_drm_buffer;
struct wl_resource;

QW_BEGIN_NAMESPACE

class QWDrmPrivate;
class QWDisplay;
class QWRenderer;
class QW_EXPORT QWDrm : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDrm)
public:
    inline wlr_drm *handle() const {
        return QWObject::handle<wlr_drm>();
    }

    static QWDrm *get(wlr_drm *handle);
    static QWDrm *from(wlr_drm *handle);
    static QWDrm *create(QWDisplay *display, QWRenderer *render);

Q_SIGNALS:
    void beforeDestroy(QWDrm *self);

private:
    QWDrm(wlr_drm *handle, bool isOwner);
    ~QWDrm() = default;
};


class QW_EXPORT QWDrmBuffer {
public:
    QWDrmBuffer() = delete;
    ~QWDrmBuffer() = delete;

    wlr_drm_buffer *handle() const;

    static QWDrmBuffer *from(wlr_drm_buffer *handle);
    static QWDrmBuffer *from(wl_resource *resource);
    static bool isDrmBufferResource(wl_resource *resource);
};

QW_END_NAMESPACE
