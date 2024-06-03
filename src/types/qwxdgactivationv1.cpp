// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgactivationv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_activation_v1.h>
}

QW_BEGIN_NAMESPACE

class QWXdgActivationV1Private : public QWWrapObjectPrivate
{
public:
    QWXdgActivationV1Private(wlr_xdg_activation_v1 *handle, bool isOwner, QWXdgActivationV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.request_activate, this, &QWXdgActivationV1Private::on_request_activate);
        sc.connect(&handle->events.new_token, this, &QWXdgActivationV1Private::on_new_token);
    }

    void on_request_activate(wlr_xdg_activation_v1_request_activate_event *);
    void on_new_token(wlr_xdg_activation_token_v1 *);

    QW_DECLARE_PUBLIC(QWXdgActivationV1)
};

void QWXdgActivationV1Private::on_request_activate(wlr_xdg_activation_v1_request_activate_event *event)
{
    Q_EMIT q_func()->requestActivate(event);
}

void QWXdgActivationV1Private::on_new_token(wlr_xdg_activation_token_v1 *token)
{
    Q_EMIT q_func()->newToken(QWXdgActivationTokenV1::from(token));
}

QWXdgActivationV1::QWXdgActivationV1(wlr_xdg_activation_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWXdgActivationV1Private(handle, isOwner, this))
{

}

QWXdgActivationV1 *QWXdgActivationV1::create(QWDisplay *display)
{
    auto *handle = wlr_xdg_activation_v1_create(display->handle());
    return handle ? from(handle) : nullptr;
}

QWXdgActivationTokenV1 *QWXdgActivationV1::createToken()
{
    auto *handle = wlr_xdg_activation_token_v1_create(this->handle());
    return handle ? QWXdgActivationTokenV1::from(handle) : nullptr;
}

QWXdgActivationTokenV1 *QWXdgActivationV1::findToken(const char *token_str) const
{
    auto *handle = wlr_xdg_activation_v1_find_token(this->handle(), token_str);
    return handle ? QWXdgActivationTokenV1::from(handle) : nullptr;
}

QWXdgActivationTokenV1 *QWXdgActivationV1::addToken(const char *token_str)
{
    auto *handle = wlr_xdg_activation_v1_add_token(this->handle(), token_str);
    return handle ? QWXdgActivationTokenV1::from(handle) : nullptr;
}

QWXdgActivationV1 *QWXdgActivationV1::get(wlr_xdg_activation_v1 *handle)
{
    return static_cast<QWXdgActivationV1*>(QWXdgActivationV1Private::map.value(handle));
}

QWXdgActivationV1 *QWXdgActivationV1::from(wlr_xdg_activation_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgActivationV1(handle, false);
}

QW_END_NAMESPACE
