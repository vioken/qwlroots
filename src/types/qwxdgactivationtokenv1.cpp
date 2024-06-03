// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgactivationv1.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_activation_v1.h>
}

QW_BEGIN_NAMESPACE

class QWXdgActivationTokenV1Private : public QWWrapObjectPrivate
{
public:
    QWXdgActivationTokenV1Private(wlr_xdg_activation_token_v1 *handle, bool isOwner, QWXdgActivationTokenV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_xdg_activation_token_v1_destroy))
    {

    }
    QW_DECLARE_PUBLIC(QWXdgActivationTokenV1)
};

QWXdgActivationTokenV1::QWXdgActivationTokenV1(wlr_xdg_activation_token_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWXdgActivationTokenV1Private(handle, isOwner, this))
{

}

const char *QWXdgActivationTokenV1::getName() const
{
    return wlr_xdg_activation_token_v1_get_name(handle());
}

QWXdgActivationTokenV1 *QWXdgActivationTokenV1::get(wlr_xdg_activation_token_v1 *handle)
{
    return static_cast<QWXdgActivationTokenV1*>(QWXdgActivationTokenV1Private::map.value(handle));
}

QWXdgActivationTokenV1 *QWXdgActivationTokenV1::from(wlr_xdg_activation_token_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgActivationTokenV1(handle, false);
}

QW_END_NAMESPACE
