// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletv2.h"
#include "qwtablet.h"
#include "private/qwglobal_p.h"

#include <qwcompositor.h>
#include <QHash>
#include <QPointF>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QWTabletV2TabletToolPrivate : public QWWrapObjectPrivate
{
public:
    QWTabletV2TabletToolPrivate(wlr_tablet_v2_tablet_tool *handle, bool isOwner, QWTabletV2TabletTool *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map)
    {
        sc.connect(&handle->events.set_cursor, this, &QWTabletV2TabletToolPrivate::on_set_cursor);
    }

    inline void on_set_cursor(wlr_tablet_v2_event_cursor *cursor) {
        Q_EMIT q_func()->setCursor(cursor);
    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWTabletV2TabletTool)
};
QHash<void*, QWWrapObject*> QWTabletV2TabletToolPrivate::map;

QWTabletV2TabletTool::QWTabletV2TabletTool(wlr_tablet_v2_tablet_tool *handle, bool isOwner, QWTabletTool *parent)
    : QWWrapObject(*new QWTabletV2TabletToolPrivate(handle, isOwner, this), parent)
{

}

QWTabletV2TabletTool *QWTabletV2TabletTool::get(wlr_tablet_v2_tablet_tool *handle)
{
    return static_cast<QWTabletV2TabletTool*>(QWTabletV2TabletToolPrivate::map.value(handle));
}

QWTabletV2TabletTool *QWTabletV2TabletTool::from(wlr_tablet_v2_tablet_tool *handle)
{
    if (auto o = get(handle))
        return o;
    auto *parent = QWTabletTool::from(handle->wlr_tool);
    return new QWTabletV2TabletTool(handle, false, parent);
}

void QWTabletV2TabletTool::sendProximityIn(QWTabletV2Tablet *tablet, QWSurface *surface)
{
    return wlr_send_tablet_v2_tablet_tool_proximity_in(handle(), tablet->handle(), surface->handle());
}

void QWTabletV2TabletTool::sendDown()
{
    return wlr_send_tablet_v2_tablet_tool_down(handle());
}

void QWTabletV2TabletTool::sendUp()
{
    return wlr_send_tablet_v2_tablet_tool_up(handle());
}

void QWTabletV2TabletTool::sendMotion(const QPointF &pos)
{
    return wlr_send_tablet_v2_tablet_tool_motion(handle(), pos.x(), pos.y());
}

void QWTabletV2TabletTool::sendPressure(double pressure)
{
    return wlr_send_tablet_v2_tablet_tool_pressure(handle(), pressure);
}

void QWTabletV2TabletTool::sendDistance(double distance)
{
    return wlr_send_tablet_v2_tablet_tool_distance(handle(), distance);
}

void QWTabletV2TabletTool::sendTilt(const QPointF &pos)
{
    return wlr_send_tablet_v2_tablet_tool_tilt(handle(), pos.x(), pos.y());
}

void QWTabletV2TabletTool::sendRotation(double degrees)
{
    return wlr_send_tablet_v2_tablet_tool_rotation(handle(), degrees);
}

void QWTabletV2TabletTool::sendSlider(double position)
{
    return wlr_send_tablet_v2_tablet_tool_slider(handle(), position);
}

void QWTabletV2TabletTool::sendWheel(double degrees, int32_t clicks)
{
    return wlr_send_tablet_v2_tablet_tool_wheel(handle(), degrees, clicks);
}

void QWTabletV2TabletTool::sendProximityOut()
{
    return wlr_send_tablet_v2_tablet_tool_proximity_out(handle());
}

void QWTabletV2TabletTool::sendButton(uint32_t button, zwp_tablet_pad_v2_button_state_t state)
{
    return wlr_send_tablet_v2_tablet_tool_button(handle(), button, static_cast<zwp_tablet_pad_v2_button_state>(state));
}

void QWTabletV2TabletTool::notifyProximityIn(QWTabletV2Tablet *tablet, QWSurface *surface)
{
    return wlr_tablet_v2_tablet_tool_notify_proximity_in(handle(), tablet->handle(), surface->handle());
}

void QWTabletV2TabletTool::notifyDown()
{
    return wlr_tablet_v2_tablet_tool_notify_down(handle());
}

void QWTabletV2TabletTool::notifyUp()
{
    return wlr_tablet_v2_tablet_tool_notify_up(handle());
}

void QWTabletV2TabletTool::notifyMotion(const QPointF &pos)
{
    return wlr_tablet_v2_tablet_tool_notify_motion(handle(), pos.x(), pos.y());
}

void QWTabletV2TabletTool::notifyPressure(double pressure)
{
    return wlr_tablet_v2_tablet_tool_notify_pressure(handle(), pressure);
}

void QWTabletV2TabletTool::notifyDistance(double distance)
{
    return wlr_tablet_v2_tablet_tool_notify_distance(handle(), distance);
}

void QWTabletV2TabletTool::notifyTilt(const QPointF &pos)
{
    return wlr_tablet_v2_tablet_tool_notify_tilt(handle(), pos.x(), pos.y());
}

void QWTabletV2TabletTool::notifyRotation(double degrees)
{
    return wlr_tablet_v2_tablet_tool_notify_rotation(handle(), degrees);
}

void QWTabletV2TabletTool::notifySlider(double position)
{
    return wlr_tablet_v2_tablet_tool_notify_slider(handle(), position);
}

void QWTabletV2TabletTool::notifyWheel(double degrees, int32_t clicks)
{
    return wlr_tablet_v2_tablet_tool_notify_wheel(handle(), degrees, clicks);
}

void QWTabletV2TabletTool::notifyProximityOut()
{
    return wlr_tablet_v2_tablet_tool_notify_proximity_out(handle());
}

void QWTabletV2TabletTool::notifyButton(uint32_t button, zwp_tablet_pad_v2_button_state_t state)
{
    return wlr_tablet_v2_tablet_tool_notify_button(handle(), button, static_cast<zwp_tablet_pad_v2_button_state>(state));
}

void QWTabletV2TabletTool::startGrab(QWTabletToolV2Grab *grab)
{
    return wlr_tablet_tool_v2_start_grab(handle(), grab->handle());
}

void QWTabletV2TabletTool::endGrab()
{
    return wlr_tablet_tool_v2_end_grab(handle());
}

void QWTabletV2TabletTool::startImplicitGrab()
{
    return wlr_tablet_tool_v2_start_implicit_grab(handle());
}

bool QWTabletV2TabletTool::hasImplicitGrab() const
{
    return wlr_tablet_tool_v2_has_implicit_grab(handle());
}

QW_END_NAMESPACE
