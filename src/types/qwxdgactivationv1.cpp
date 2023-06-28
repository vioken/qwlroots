// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgactivationv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_activation_v1.h>
}

QW_BEGIN_NAMESPACE

class QWXdgActivationV1Private : public QWObjectPrivate
{
public:
    QWXdgActivationV1Private(wlr_xdg_activation_v1 *handle, bool isOwner, QWXdgActivationV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgActivationV1Private::on_destroy);
        sc.connect(&handle->events.request_activate, this, &QWXdgActivationV1Private::on_request_activate);
#if WLR_VERSION_MINOR > 16
        sc.connect(&handle->events.new_token, this, &QWXdgActivationV1Private::on_new_token);
#endif
    }
    ~QWXdgActivationV1Private() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_request_activate(wlr_xdg_activation_v1_request_activate_event *);
#if WLR_VERSION_MINOR > 16
    void on_new_token(wlr_xdg_activation_token_v1 *);
#endif

    static QHash<void*, QWXdgActivationV1*> map;
    QW_DECLARE_PUBLIC(QWXdgActivationV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgActivationV1*> QWXdgActivationV1Private::map;

void QWXdgActivationV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWXdgActivationV1Private::on_request_activate(wlr_xdg_activation_v1_request_activate_event *event)
{
    Q_EMIT q_func()->requestActivate(event);
}

#if WLR_VERSION_MINOR > 16
void QWXdgActivationV1Private::on_new_token(wlr_xdg_activation_token_v1 *token)
{
    Q_EMIT q_func()->newToken(QWXdgActivationTokenV1::from(token));
}
#endif

QWXdgActivationV1::QWXdgActivationV1(wlr_xdg_activation_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgActivationV1Private(handle, isOwner, this))
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
    return QWXdgActivationV1Private::map.value(handle);
}

QWXdgActivationV1 *QWXdgActivationV1::from(wlr_xdg_activation_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgActivationV1(handle, false);
}

QW_END_NAMESPACE
