// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwgammacontorlv1.h"
#include "qwdisplay.h"
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
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.set_gamma, this, &QWGammaControlManagerV1Private::on_set_gamma);
    }

    void on_set_gamma(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWGammaControlManagerV1)
};
QHash<void*, QWWrapObject*> QWGammaControlManagerV1Private::map;

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

QW_END_NAMESPACE
