// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_pointer_gestures_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSeat;
class QWPointerGesturesV1Private;
class QW_EXPORT QWPointerGesturesV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPointerGesturesV1)
public:
    inline wlr_pointer_gestures_v1 *handle() const {
        return QWObject::handle<wlr_pointer_gestures_v1>();
    }

    static QWPointerGesturesV1 *get(wlr_pointer_gestures_v1 *handle);
    static QWPointerGesturesV1 *from(wlr_pointer_gestures_v1 *handle);
    static QWPointerGesturesV1 *create(QWDisplay *display);

    void sendSwipeBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers);
    void sendSwipeUpdate(QWSeat *seat, uint32_t time_msec, const QPointF &pos);
    void sendSwipeEnd(QWSeat *seat, uint32_t time_msec,bool cancelled);
    void sendPinchBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers);
    void sendPinchUpdate(QWSeat *seat, uint32_t time_msec, const QPointF &pos, double scale, double rotation);
    void sendPinchEnd(QWSeat *seat, uint32_t time_msec,bool cancelled);
    void sendHoldBegin(QWSeat *seat, uint32_t time_msec, uint32_t fingers);
    void sendHoldEnd(QWSeat *seat, uint32_t time_msec, bool cancelled);

Q_SIGNALS:
    void beforeDestroy(QWPointerGesturesV1 *self);

private:
    QWPointerGesturesV1(wlr_pointer_gestures_v1 *handle, bool isOwner);
    ~QWPointerGesturesV1() = default;
};

QW_END_NAMESPACE

