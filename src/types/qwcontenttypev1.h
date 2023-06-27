// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_content_type_manager_v1;
typedef uint32_t wp_content_type_v1_type_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSurface;
class QWContentTypeManagerV1Private;
class QW_EXPORT QWContentTypeManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWContentTypeManagerV1)
public:
    inline wlr_content_type_manager_v1 *handle() const {
        return QWObject::handle<wlr_content_type_manager_v1>();
    }

    static QWContentTypeManagerV1 *get(wlr_content_type_manager_v1 *handle);
    static QWContentTypeManagerV1 *from(wlr_content_type_manager_v1 *handle);
    static QWContentTypeManagerV1 *create(QWDisplay *display, uint32_t version);

    wp_content_type_v1_type_t getSurfaceContentType(QWSurface *surface);

Q_SIGNALS:
    void beforeDestroy(QWContentTypeManagerV1 *self);

private:
    QWContentTypeManagerV1(wlr_content_type_manager_v1 *handle, bool isOwner);
    ~QWContentTypeManagerV1() = default;
};

QW_END_NAMESPACE

