// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwfractionalscalemanagerv1.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_fractional_scale_v1.h>
}

QW_BEGIN_NAMESPACE

class QWFractionalScaleManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWFractionalScaleManagerV1Private(wlr_fractional_scale_manager_v1 *handle, bool isOwner, QWFractionalScaleManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWFractionalScaleManagerV1)
};
QHash<void*, QWWrapObject*> QWFractionalScaleManagerV1Private::map;

QWFractionalScaleManagerV1::QWFractionalScaleManagerV1(wlr_fractional_scale_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWFractionalScaleManagerV1Private(handle, isOwner, this))
{

}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::get(wlr_fractional_scale_manager_v1 *handle)
{
    return static_cast<QWFractionalScaleManagerV1*>(QWFractionalScaleManagerV1Private::map.value(handle));
}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::from(wlr_fractional_scale_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWFractionalScaleManagerV1(handle, false);
}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::create(QWDisplay *display, uint32_t version)
{
    auto *handle = wlr_fractional_scale_manager_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWFractionalScaleManagerV1(handle, true);
}

void QWFractionalScaleManagerV1::notifyScale(QWSurface *surface, double scale)
{
    wlr_fractional_scale_v1_notify_scale(surface->handle(), scale);
}

QW_END_NAMESPACE
