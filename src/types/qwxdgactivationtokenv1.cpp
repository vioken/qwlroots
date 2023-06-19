// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgactivationv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_activation_v1.h>
}

QW_BEGIN_NAMESPACE

class QWXdgActivationTokenV1Private : public QWObjectPrivate
{
public:
    QWXdgActivationTokenV1Private(wlr_xdg_activation_token_v1 *handle, bool isOwner, QWXdgActivationTokenV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgActivationTokenV1Private::on_destroy);
    }
    ~QWXdgActivationTokenV1Private() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_xdg_activation_token_v1_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWXdgActivationTokenV1*> map;
    QW_DECLARE_PUBLIC(QWXdgActivationTokenV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgActivationTokenV1*> QWXdgActivationTokenV1Private::map;

void QWXdgActivationTokenV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWXdgActivationTokenV1::QWXdgActivationTokenV1(wlr_xdg_activation_token_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgActivationTokenV1Private(handle, isOwner, this))
{

}

const char *QWXdgActivationTokenV1::getName() const
{
    return wlr_xdg_activation_token_v1_get_name(handle());
}

QWXdgActivationTokenV1 *QWXdgActivationTokenV1::get(wlr_xdg_activation_token_v1 *handle)
{
    return QWXdgActivationTokenV1Private::map.value(handle);
}

QWXdgActivationTokenV1 *QWXdgActivationTokenV1::from(wlr_xdg_activation_token_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgActivationTokenV1(handle, false);
}

QW_END_NAMESPACE
