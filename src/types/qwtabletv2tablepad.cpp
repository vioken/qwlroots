// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputdevice.h"
#include "qwtabletv2.h"
#include "private/qwglobal_p.h"

#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

QW_BEGIN_NAMESPACE

class QWTabletV2TabletPadPrivate : public QWWrapObjectPrivate
{
public:
    QWTabletV2TabletPadPrivate(wlr_tablet_v2_tablet_pad *handle, bool isOwner, QWTabletV2TabletPad *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq)
    {
        sc.connect(&handle->events.button_feedback, this, &QWTabletV2TabletPadPrivate::on_button_feedback);
        sc.connect(&handle->events.ring_feedback, this, &QWTabletV2TabletPadPrivate::on_ring_feedback);
        sc.connect(&handle->events.strip_feedback, this, &QWTabletV2TabletPadPrivate::on_strip_feedback);
    }

    void on_button_feedback(wlr_tablet_v2_event_feedback *);
    void on_ring_feedback(wlr_tablet_v2_event_feedback *);
    void on_strip_feedback(wlr_tablet_v2_event_feedback *);

    QW_DECLARE_PUBLIC(QWTabletV2TabletPad)
};

void QWTabletV2TabletPadPrivate::on_button_feedback(wlr_tablet_v2_event_feedback *event)
{
    Q_EMIT q_func()->buttonFeedback(event);
}

void QWTabletV2TabletPadPrivate::on_ring_feedback(wlr_tablet_v2_event_feedback *event)
{
    Q_EMIT q_func()->ringFeedback(event);
}

void QWTabletV2TabletPadPrivate::on_strip_feedback(wlr_tablet_v2_event_feedback *event)
{
    Q_EMIT q_func()->stripFeedback(event);
}

QWTabletV2TabletPad::QWTabletV2TabletPad(wlr_tablet_v2_tablet_pad *handle, bool isOwner, QWInputDevice *parent)
    : QWWrapObject(*new QWTabletV2TabletPadPrivate(handle, isOwner, this), parent)
{

}

QWTabletV2TabletPad *QWTabletV2TabletPad::get(wlr_tablet_v2_tablet_pad *handle)
{
    return static_cast<QWTabletV2TabletPad*>(QWTabletV2TabletPadPrivate::map.value(handle));
}

QWTabletV2TabletPad *QWTabletV2TabletPad::from(wlr_tablet_v2_tablet_pad *handle)
{
    if (auto o = get(handle))
        return o;
    auto *parent = QWInputDevice::from(handle->wlr_device);
    return new QWTabletV2TabletPad(handle, false, parent);
}

uint32_t QWTabletV2TabletPad::sendEnter(QWTabletV2Tablet *tablet, QWSurface *surface)
{
    return wlr_send_tablet_v2_tablet_pad_enter(handle(), tablet->handle(), surface->handle());
}

void QWTabletV2TabletPad::sendButton(size_t button, uint32_t time, zwp_tablet_pad_v2_button_state_t state)
{
    return wlr_send_tablet_v2_tablet_pad_button(handle(), button, time, static_cast<zwp_tablet_pad_v2_button_state>(state));
}

void QWTabletV2TabletPad::sendStrip(uint32_t strip, double position, bool finger, uint32_t time)
{
    return wlr_send_tablet_v2_tablet_pad_strip(handle(), strip, position, finger, time);
}

void QWTabletV2TabletPad::sendRing(uint32_t ring, double position, bool finger, uint32_t time)
{
    return wlr_send_tablet_v2_tablet_pad_ring(handle(), ring, position, finger, time);
}

uint32_t QWTabletV2TabletPad::sendLeave(QWSurface *surface)
{
    return wlr_send_tablet_v2_tablet_pad_leave(handle(), surface->handle());
}

uint32_t QWTabletV2TabletPad::sendMode(size_t group, uint32_t mode, uint32_t time)
{
    return wlr_send_tablet_v2_tablet_pad_mode(handle(), group, mode, time);
}

uint32_t QWTabletV2TabletPad::notifyEnter(QWTabletV2Tablet *tablet, QWSurface *surface)
{
    return wlr_tablet_v2_tablet_pad_notify_enter(handle(), tablet->handle(), surface->handle());
}

void QWTabletV2TabletPad::notifyButton(size_t button, uint32_t time, zwp_tablet_pad_v2_button_state_t state)
{
    return wlr_tablet_v2_tablet_pad_notify_button(handle(), button, time, static_cast<zwp_tablet_pad_v2_button_state>(state));
}

void QWTabletV2TabletPad::notifyStrip(uint32_t strip, double position, bool finger, uint32_t time)
{
    return wlr_tablet_v2_tablet_pad_notify_strip(handle(), strip, position, finger, time);
}

void QWTabletV2TabletPad::notifyRing(uint32_t ring, double position, bool finger, uint32_t time)
{
    return wlr_tablet_v2_tablet_pad_notify_ring(handle(), ring, position, finger, time);
}

uint32_t QWTabletV2TabletPad::notifyLeave(QWSurface *surface)
{
    return wlr_tablet_v2_tablet_pad_notify_leave(handle(), surface->handle());
}

uint32_t QWTabletV2TabletPad::notifyMode(size_t group, uint32_t mode, uint32_t time)
{
    return wlr_tablet_v2_tablet_pad_notify_mode(handle(), group, mode, time);
}

void QWTabletV2TabletPad::endGrab()
{
    return wlr_tablet_v2_end_grab(handle());
}

void QWTabletV2TabletPad::startGrab(QWTabletPadV2Grab *grab)
{
    return wlr_tablet_v2_start_grab(handle(), grab->handle());
}

QW_END_NAMESPACE
