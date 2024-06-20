// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwgammacontorlv1.h"
#include "qwdisplay.h"
#include "qwoutput.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_gamma_control_v1.h>
}

QW_BEGIN_NAMESPACE

class QWGammaControlManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWGammaControlManagerV1Private(wlr_gamma_control_manager_v1 *handle, bool isOwner, QWGammaControlManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.set_gamma, this, &QWGammaControlManagerV1Private::on_set_gamma);
    }

    void on_set_gamma(void *);

    QW_DECLARE_PUBLIC(QWGammaControlManagerV1)
};

void QWGammaControlManagerV1Private::on_set_gamma(void *data)
{
    Q_EMIT q_func()->gammaChanged(reinterpret_cast<wlr_gamma_control_manager_v1_set_gamma_event*>(data));
}

QWGammaControlManagerV1::QWGammaControlManagerV1(wlr_gamma_control_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWGammaControlManagerV1Private(handle, isOwner, this))
{

}

QWGammaControlManagerV1 *QWGammaControlManagerV1::get(wlr_gamma_control_manager_v1 *handle)
{
    return static_cast<QWGammaControlManagerV1*>(QWGammaControlManagerV1Private::map.value(handle));
}

QWGammaControlManagerV1 *QWGammaControlManagerV1::from(wlr_gamma_control_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWGammaControlManagerV1(handle, false);
}

QWGammaControlManagerV1 *QWGammaControlManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_gamma_control_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWGammaControlManagerV1(handle, true);
}

wlr_gamma_control_v1 *QWGammaControl::handle() const
{
    return reinterpret_cast<wlr_gamma_control_v1*>(const_cast<QWGammaControl*>(this));
}

QWGammaControl *QWGammaControl::from(wlr_gamma_control_v1 *handle)
{
    return reinterpret_cast<QWGammaControl*>(handle);
}

void QWGammaControl::sendFailedAndDestroy()
{
    wlr_gamma_control_v1_send_failed_and_destroy(handle());
}

bool QWGammaControl::gammaControlApply(wlr_output_state *output_state)
{
    return wlr_gamma_control_v1_apply(handle(), output_state);
}

QWGammaControl *QWGammaControlManagerV1::getControl(QWOutput *output)
{
    return QWGammaControl::from(wlr_gamma_control_manager_v1_get_control(handle(), output->handle()));
}

QW_END_NAMESPACE
