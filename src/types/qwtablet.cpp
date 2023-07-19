// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtablet.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QWTabletPrivate : public QWInputDevicePrivate
{
public:
    QWTabletPrivate(wlr_tablet *handle, bool isOwner, QWTablet *qq)
        : QWInputDevicePrivate(&handle->base, isOwner, qq)
    {
        sc.connect(&handle->events.axis, this, &QWTabletPrivate::on_axis);
        sc.connect(&handle->events.proximity, this, &QWTabletPrivate::on_proximity);
        sc.connect(&handle->events.tip, this, &QWTabletPrivate::on_tip);
        sc.connect(&handle->events.button, this, &QWTabletPrivate::on_button);
    }
    ~QWTabletPrivate() override = default;

    void on_axis(void *);
    void on_proximity(void *);
    void on_tip(void *);
    void on_button(void *);

    QW_DECLARE_PUBLIC(QWTablet)
};

void QWTabletPrivate::on_axis(void *)
{
    Q_EMIT q_func()->axis();
}

void QWTabletPrivate::on_proximity(void *)
{
    Q_EMIT q_func()->proximity();
}

void QWTabletPrivate::on_tip(void *)
{
    Q_EMIT q_func()->tip();
}

void QWTabletPrivate::on_button(void *)
{
    Q_EMIT q_func()->button();
}

QWTablet::QWTablet(wlr_tablet *handle, bool isOwner)
    : QWInputDevice(*new QWTabletPrivate(handle, isOwner, this)) {}

QWTablet *QWTablet::get(wlr_tablet *handle)
{
    return qobject_cast<QWTablet*>(QWInputDevice::get(&handle->base));
}

QWTablet *QWTablet::from(wlr_tablet *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTablet(handle, false);
}

QWTablet *QWTablet::fromInputDevice(wlr_input_device *input_device)
{
    wlr_tablet *handle = wlr_tablet_from_input_device(input_device);
    return from(handle);
}

QW_END_NAMESPACE
