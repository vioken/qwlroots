// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointergesturesv1.h"
#include "private/qwglobal_p.h"

#include <qwseat.h>
#include <qwdisplay.h>
#include <QHash>
#include <QPointF>

extern "C" {
#include <wlr/types/wlr_pointer_gestures_v1.h>
}

QW_BEGIN_NAMESPACE

class QWPointerGesturesV1Private : public QWWrapObjectPrivate
{
public:
    QWPointerGesturesV1Private(wlr_pointer_gestures_v1 *handle, bool isOwner, QWPointerGesturesV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWPointerGesturesV1)
};

QWPointerGesturesV1::QWPointerGesturesV1(wlr_pointer_gestures_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWPointerGesturesV1Private(handle, isOwner, this))
{

}

QWPointerGesturesV1 *QWPointerGesturesV1::get(wlr_pointer_gestures_v1 *handle)
{
    return static_cast<QWPointerGesturesV1*>(QWPointerGesturesV1Private::map.value(handle));
}

QWPointerGesturesV1 *QWPointerGesturesV1::from(wlr_pointer_gestures_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPointerGesturesV1(handle, false);
}

QWPointerGesturesV1 *QWPointerGesturesV1::create(QWDisplay *display)
{
    auto *handle = wlr_pointer_gestures_v1_create(display->handle());
    return handle ? new QWPointerGesturesV1(handle, true) : nullptr;
}

void QWPointerGesturesV1::sendSwipeBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers)
{
    wlr_pointer_gestures_v1_send_swipe_begin(handle(), seat->handle(), time_msec, fingers);
}

void QWPointerGesturesV1::sendSwipeUpdate(QWSeat *seat, uint32_t time_msec, const QPointF &pos)
{
    wlr_pointer_gestures_v1_send_swipe_update(handle(), seat->handle(), time_msec, pos.x(), pos.y());
}

void QWPointerGesturesV1::sendSwipeEnd(QWSeat *seat, uint32_t time_msec,bool cancelled)
{
    wlr_pointer_gestures_v1_send_swipe_end(handle(), seat->handle(), time_msec, cancelled);
}

void QWPointerGesturesV1::sendPinchBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers)
{
    wlr_pointer_gestures_v1_send_pinch_begin(handle(), seat->handle(), time_msec, fingers);
}

void QWPointerGesturesV1::sendPinchUpdate(QWSeat *seat, uint32_t time_msec, const QPointF &pos, double scale, double rotation)
{
    wlr_pointer_gestures_v1_send_pinch_update(handle(), seat->handle(), time_msec, pos.x(), pos.y(), scale, rotation);
}

void QWPointerGesturesV1::sendPinchEnd(QWSeat *seat, uint32_t time_msec,bool cancelled)
{
    wlr_pointer_gestures_v1_send_pinch_end(handle(), seat->handle(), time_msec, cancelled);
}

void QWPointerGesturesV1::sendHoldBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers)
{
    wlr_pointer_gestures_v1_send_hold_begin(handle(), seat->handle(), time_msec, fingers);
}

void QWPointerGesturesV1::sendHoldEnd(QWSeat *seat, uint32_t time_msec, bool cancelled)
{
    wlr_pointer_gestures_v1_send_hold_end(handle(), seat->handle(), time_msec, cancelled);
}

QW_END_NAMESPACE
