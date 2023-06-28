// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletv2.h"
#include "qwinputdevice.h"
#include "util/qwsignalconnector.h"

#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QWTabletV2TabletPrivate : public QWObjectPrivate
{
public:
    QWTabletV2TabletPrivate(wlr_tablet_v2_tablet *handle, bool isOwner, QWTabletV2Tablet *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
    }
    ~QWTabletV2TabletPrivate() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWTabletV2Tablet*> map;
    QW_DECLARE_PUBLIC(QWTabletV2Tablet)
    QWSignalConnector sc;
};
QHash<void*, QWTabletV2Tablet*> QWTabletV2TabletPrivate::map;

void QWTabletV2TabletPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWTabletV2Tablet::QWTabletV2Tablet(wlr_tablet_v2_tablet *handle, bool isOwner, QWInputDevice *parent)
    : QObject(parent)
    , QWObject(*new QWTabletV2TabletPrivate(handle, isOwner, this))
{
}

QWTabletV2Tablet *QWTabletV2Tablet::get(wlr_tablet_v2_tablet *handle)
{
    return QWTabletV2TabletPrivate::map.value(handle);
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
