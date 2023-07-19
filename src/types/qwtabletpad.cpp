// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletpad.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_tablet_pad.h>
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QWTabletPadPrivate : public QWInputDevicePrivate
{
public:
    QWTabletPadPrivate(wlr_tablet_pad *handle, bool isOwner, QWTabletPad *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.button, this, &QWTabletPadPrivate::on_button);
        sc.connect(&handle->events.ring, this, &QWTabletPadPrivate::on_ring);
        sc.connect(&handle->events.strip, this, &QWTabletPadPrivate::on_strip);
        sc.connect(&handle->events.attach_tablet, this, &QWTabletPadPrivate::on_attach_tablet);
    }
    ~QWTabletPadPrivate() override = default;

    void on_button(void *);
    void on_ring(void *);
    void on_strip(void *);
    void on_attach_tablet(void *);

    QW_DECLARE_PUBLIC(QWTabletPad)
};

void QWTabletPadPrivate::on_button(void *)
{
    Q_EMIT q_func()->button();
}

void QWTabletPadPrivate::on_ring(void *)
{
    Q_EMIT q_func()->ring();
}

void QWTabletPadPrivate::on_strip(void *)
{
    Q_EMIT q_func()->strip();
}

void QWTabletPadPrivate::on_attach_tablet(void *data)
{
    Q_EMIT q_func()->attachTablet(static_cast<wlr_tablet_tool*>(data));
}

QWTabletPad::QWTabletPad(wlr_tablet_pad *handle, bool isOwner)
    : QWInputDevice(*new QWTabletPadPrivate(handle, isOwner, this)) {}

QWTabletPad *QWTabletPad::get(wlr_tablet_pad *handle)
{
    return qobject_cast<QWTabletPad*>(QWInputDevice::get(&handle->base));
}

QWTabletPad *QWTabletPad::from(wlr_tablet_pad *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTabletPad(handle, false);
}

QWTabletPad *QWTabletPad::fromInputDevice(wlr_input_device *input_device)
{
    wlr_tablet_pad *handle = wlr_tablet_pad_from_input_device(input_device);
    return from(handle);
}

QW_END_NAMESPACE
