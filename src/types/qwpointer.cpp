// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointer.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_pointer.h>
}

QW_BEGIN_NAMESPACE

class QWPointerPrivate : public QWInputDevicePrivate
{
public:
    QWPointerPrivate(wlr_pointer *handle, bool isOwner, QWPointer *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.motion, this, &QWPointerPrivate::on_motion);
        sc.connect(&handle->events.motion_absolute, this, &QWPointerPrivate::on_motion_absolute);
        sc.connect(&handle->events.button, this, &QWPointerPrivate::on_button);
        sc.connect(&handle->events.axis, this, &QWPointerPrivate::on_axis);
        sc.connect(&handle->events.frame, this, &QWPointerPrivate::on_frame);

        sc.connect(&handle->events.swipe_begin, this, &QWPointerPrivate::on_swipe_begin);
        sc.connect(&handle->events.swipe_update, this, &QWPointerPrivate::on_swipe_update);
        sc.connect(&handle->events.swipe_end, this, &QWPointerPrivate::on_swipe_end);

        sc.connect(&handle->events.pinch_begin, this, &QWPointerPrivate::on_pinch_begin);
        sc.connect(&handle->events.pinch_update, this, &QWPointerPrivate::on_pinch_update);
        sc.connect(&handle->events.pinch_end, this, &QWPointerPrivate::on_pinch_end);

        sc.connect(&handle->events.hold_begin, this, &QWPointerPrivate::on_hold_begin);
        sc.connect(&handle->events.hold_end, this, &QWPointerPrivate::on_hold_end);
    }
    ~QWPointerPrivate() override = default;

    void on_motion(void *);
    void on_motion_absolute(void *);
    void on_button(void *);
    void on_axis(void *);
    void on_frame(void *);

    void on_swipe_begin(void *);
    void on_swipe_update(void *);
    void on_swipe_end(void *);

    void on_pinch_begin(void *);
    void on_pinch_update(void *);
    void on_pinch_end(void *);

    void on_hold_begin(void *);
    void on_hold_end(void *);

    QW_DECLARE_PUBLIC(QWPointer)
};

void QWPointerPrivate::on_motion(void *data)
{
    Q_EMIT q_func()->motion(static_cast<wlr_pointer_motion_event*>(data));
}

void QWPointerPrivate::on_motion_absolute(void *data)
{
    Q_EMIT q_func()->motionAbsolute(static_cast<wlr_pointer_motion_absolute_event*>(data));
}

void QWPointerPrivate::on_button(void *data)
{
    Q_EMIT q_func()->button(static_cast<wlr_pointer_button_event*>(data));
}

void QWPointerPrivate::on_axis(void *data)
{
    Q_EMIT q_func()->axis(static_cast<wlr_pointer_axis_event*>(data));
}

void QWPointerPrivate::on_frame(void *)
{
    Q_EMIT q_func()->frame();
}

void QWPointerPrivate::on_swipe_begin(void *data)
{
    Q_EMIT q_func()->swipeBegin(static_cast<wlr_pointer_swipe_begin_event*>(data));
}

void QWPointerPrivate::on_swipe_update(void *data)
{
    Q_EMIT q_func()->swipeUpdate(static_cast<wlr_pointer_swipe_update_event*>(data));
}

void QWPointerPrivate::on_swipe_end(void *data)
{
    Q_EMIT q_func()->swipeEnd(static_cast<wlr_pointer_swipe_end_event*>(data));
}

void QWPointerPrivate::on_pinch_begin(void *data)
{
    Q_EMIT q_func()->pinchBegin(static_cast<wlr_pointer_pinch_begin_event*>(data));
}

void QWPointerPrivate::on_pinch_update(void *data)
{
    Q_EMIT q_func()->pinchUpdate(static_cast<wlr_pointer_pinch_update_event*>(data));
}

void QWPointerPrivate::on_pinch_end(void *data)
{
    Q_EMIT q_func()->pinchEnd(static_cast<wlr_pointer_pinch_end_event*>(data));
}

void QWPointerPrivate::on_hold_begin(void *data)
{
    Q_EMIT q_func()->holdBegin(static_cast<wlr_pointer_hold_begin_event*>(data));
}

void QWPointerPrivate::on_hold_end(void *data)
{
    Q_EMIT q_func()->holdEnd(static_cast<wlr_pointer_hold_end_event*>(data));
}

QWPointer::QWPointer(wlr_pointer *handle, bool isOwner)
    : QWInputDevice(*new QWPointerPrivate(handle, isOwner, this)) {}

QWPointer *QWPointer::get(wlr_pointer *handle)
{
    return qobject_cast<QWPointer*>(QWInputDevice::get(&handle->base));
}

QWPointer *QWPointer::from(wlr_pointer *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPointer(handle, false);
}

QWPointer *QWPointer::fromInputDevice(wlr_input_device *input_device)
{
    wlr_pointer *handle = wlr_pointer_from_input_device(input_device);
    return from(handle);
}

QW_END_NAMESPACE
