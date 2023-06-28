// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletv2.h"
#include "qwtablet.h"
#include "util/qwsignalconnector.h"

#include <qwinputdevice.h>
#include <qwseat.h>
#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QWTabletManagerV2Private : public QWObjectPrivate
{
public:
    QWTabletManagerV2Private(wlr_tablet_manager_v2 *handle, bool isOwner, QWTabletManagerV2 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWTabletManagerV2Private::on_destroy);
    }
    ~QWTabletManagerV2Private() {
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

    static QHash<void*, QWTabletManagerV2*> map;
    QW_DECLARE_PUBLIC(QWTabletManagerV2)
    QWSignalConnector sc;
};
QHash<void*, QWTabletManagerV2*> QWTabletManagerV2Private::map;

void QWTabletManagerV2Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWTabletManagerV2::QWTabletManagerV2(wlr_tablet_manager_v2 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWTabletManagerV2Private(handle, isOwner, this))
{

}

QWTabletManagerV2 *QWTabletManagerV2::get(wlr_tablet_manager_v2 *handle)
{
    return QWTabletManagerV2Private::map.value(handle);
}

QWTabletManagerV2 *QWTabletManagerV2::from(wlr_tablet_manager_v2 *handle)
{
    if (auto *o = get(handle))
        return o;
    return new QWTabletManagerV2(handle, false);
}

QWTabletManagerV2 *QWTabletManagerV2::create(QWDisplay *display)
{
    auto *handle = wlr_tablet_v2_create(display->handle());
    return handle ? new QWTabletManagerV2(handle, true) : nullptr;
}

QWTabletV2Tablet *QWTabletManagerV2::createTablet(QWSeat *wlr_seat, QWInputDevice *wlr_device)
{
    auto *handle = wlr_tablet_create(this->handle(), wlr_seat->handle(), wlr_device->handle());
    if (!handle)
        return nullptr;
    auto *parent = QWInputDevice::from(handle->wlr_device);
    return new QWTabletV2Tablet(handle, true, parent);
}

QWTabletV2TabletPad *QWTabletManagerV2::createPad(QWSeat *wlr_seat, QWInputDevice *wlr_device)
{
    auto *handle = wlr_tablet_pad_create(this->handle(), wlr_seat->handle(), wlr_device->handle());
    if (!handle)
        return nullptr;
    auto *parent = QWInputDevice::from(handle->wlr_device);
    return new QWTabletV2TabletPad(handle, true, parent);
}

QWTabletV2TabletTool *QWTabletManagerV2::createTool(QWSeat *wlr_seat, wlr_tablet_tool *wlr_tool)
{
    auto *handle = wlr_tablet_tool_create(this->handle(), wlr_seat->handle(), wlr_tool);
    if (!handle)
        return nullptr;
    auto *parent = QWTabletTool::from(handle->wlr_tool);
    return new QWTabletV2TabletTool(handle, true, parent);
}

QW_END_NAMESPACE
