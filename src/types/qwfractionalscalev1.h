// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_fractional_scale_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSurface;
class QWFractionalScaleManagerV1Private;
class QW_EXPORT QWFractionalScaleManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWFractionalScaleManagerV1)
public:
    inline wlr_fractional_scale_manager_v1 *handle() const {
        return QWObject::handle<wlr_fractional_scale_manager_v1>();
    }

    static QWFractionalScaleManagerV1 *get(wlr_fractional_scale_manager_v1 *handle);
    static QWFractionalScaleManagerV1 *from(wlr_fractional_scale_manager_v1 *handle);
    static QWFractionalScaleManagerV1 *create(QWDisplay *display, uint32_t version);

    void notifyScale(QWSurface *surface, double scale);

Q_SIGNALS:
    void beforeDestroy(QWFractionalScaleManagerV1 *self);

private:
    QWFractionalScaleManagerV1(wlr_fractional_scale_manager_v1 *handle, bool isOwner);
    ~QWFractionalScaleManagerV1() = default;
};

QW_END_NAMESPACE

