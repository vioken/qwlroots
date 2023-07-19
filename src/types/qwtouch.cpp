// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtouch.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_touch.h>
}

QW_BEGIN_NAMESPACE

class QWTouchPrivate : public QWInputDevicePrivate
{
public:
    QWTouchPrivate(wlr_touch *handle, bool isOwner, QWTouch *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.down, this, &QWTouchPrivate::on_down);
        sc.connect(&handle->events.up, this, &QWTouchPrivate::on_up);
        sc.connect(&handle->events.motion, this, &QWTouchPrivate::on_motion);
        sc.connect(&handle->events.cancel, this, &QWTouchPrivate::on_cancel);
        sc.connect(&handle->events.frame, this, &QWTouchPrivate::on_frame);
    }
    ~QWTouchPrivate() override = default;

    void on_down(void *);
    void on_up(void *);
    void on_motion(void *);
    void on_cancel(void *);
    void on_frame(void *);

    QW_DECLARE_PUBLIC(QWTouch)
};

void QWTouchPrivate::on_down(void *data)
{
    Q_EMIT q_func()->down(reinterpret_cast<wlr_touch_down_event*>(data));
}

void QWTouchPrivate::on_up(void *data)
{
    Q_EMIT q_func()->up(reinterpret_cast<wlr_touch_up_event*>(data));
}

void QWTouchPrivate::on_motion(void *data)
{
    Q_EMIT q_func()->motion(reinterpret_cast<wlr_touch_motion_event*>(data));
}

void QWTouchPrivate::on_cancel(void *data)
{
    Q_EMIT q_func()->cancel(reinterpret_cast<wlr_touch_cancel_event*>(data));
}

void QWTouchPrivate::on_frame(void *)
{
    Q_EMIT q_func()->frame();
}

QWTouch::QWTouch(wlr_touch *handle, bool isOwner)
    : QWInputDevice(*new QWTouchPrivate(handle, isOwner, this)) {}

QWTouch *QWTouch::get(wlr_touch *handle)
{
    return qobject_cast<QWTouch*>(QWInputDevice::get(&handle->base));
}

QWTouch *QWTouch::from(wlr_touch *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTouch(handle, false);
}

QWTouch *QWTouch::fromInputDevice(wlr_input_device *input_device)
{
    auto *handle = wlr_touch_from_input_device(input_device);
    return from(handle);
}

QW_END_NAMESPACE
