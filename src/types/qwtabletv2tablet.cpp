// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletv2.h"
#include "qwinputdevice.h"
#include "private/qwglobal_p.h"

#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QWTabletV2TabletPrivate : public QWWrapObjectPrivate
{
public:
    QWTabletV2TabletPrivate(wlr_tablet_v2_tablet *handle, bool isOwner, QWTabletV2Tablet *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWTabletV2Tablet)
};
QHash<void*, QWWrapObject*> QWTabletV2TabletPrivate::map;

QWTabletV2Tablet::QWTabletV2Tablet(wlr_tablet_v2_tablet *handle, bool isOwner, QWInputDevice *parent)
    : QWWrapObject(*new QWTabletV2TabletPrivate(handle, isOwner, this), parent)
{
}

QWTabletV2Tablet *QWTabletV2Tablet::get(wlr_tablet_v2_tablet *handle)
{
    return static_cast<QWTabletV2Tablet*>(QWTabletV2TabletPrivate::map.value(handle));
}

QWTabletV2Tablet *QWTabletV2Tablet::from(wlr_tablet_v2_tablet *handle)
{
    if (auto *o = get(handle))
        return o;
    auto *parent = QWInputDevice::from(handle->wlr_device);
    return new QWTabletV2Tablet(handle, false, parent);
}

bool QWTabletV2Tablet::canAcceptTablet(QWSurface *surface) const
{
    return wlr_surface_accepts_tablet_v2(handle(), surface->handle());
}

QW_END_NAMESPACE
