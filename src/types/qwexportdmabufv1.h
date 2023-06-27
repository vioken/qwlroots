// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_export_dmabuf_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWExportDmabufManagerV1Private;
class QW_EXPORT QWExportDmabufManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWExportDmabufManagerV1)
public:
    inline wlr_export_dmabuf_manager_v1 *handle() const {
        return QWObject::handle<wlr_export_dmabuf_manager_v1>();
    }

    static QWExportDmabufManagerV1 *get(wlr_export_dmabuf_manager_v1 *handle);
    static QWExportDmabufManagerV1 *from(wlr_export_dmabuf_manager_v1 *handle);
    static QWExportDmabufManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWExportDmabufManagerV1 *self);

private:
    QWExportDmabufManagerV1(wlr_export_dmabuf_manager_v1 *handle, bool isOwner);
    ~QWExportDmabufManagerV1() = default;
};

QW_END_NAMESPACE

