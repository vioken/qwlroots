// Copyright (C) 2024 YaoBing Xiao <xiaoyaobing@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsecuritycontextmanagerv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_security_context_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSecurityContextManagerV1Private : public QWObjectPrivate
{
public:
    QWSecurityContextManagerV1Private(wlr_security_context_manager_v1 *handle, bool isOwner, QWSecurityContextManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSecurityContextManagerV1Private::on_destroy);
    }
    ~QWSecurityContextManagerV1Private() {
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

    static QHash<void*, QWSecurityContextManagerV1*> map;
    QW_DECLARE_PUBLIC(QWSecurityContextManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWSecurityContextManagerV1*> QWSecurityContextManagerV1Private::map;

void QWSecurityContextManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWSecurityContextManagerV1::QWSecurityContextManagerV1(wlr_security_context_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSecurityContextManagerV1Private(handle, isOwner, this))
{

}

QWSecurityContextManagerV1 *QWSecurityContextManagerV1::get(wlr_security_context_manager_v1 *handle)
{
    return QWSecurityContextManagerV1Private::map.value(handle);
}

QWSecurityContextManagerV1 *QWSecurityContextManagerV1::from(wlr_security_context_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSecurityContextManagerV1(handle, false);
}

QWSecurityContextManagerV1 *QWSecurityContextManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_security_context_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWSecurityContextManagerV1(handle, true);
}

const wlr_security_context_v1_state *QWSecurityContextManagerV1::lookupClient(QWSecurityContextManagerV1 *manager, wl_client *client)
{
    return wlr_security_context_manager_v1_lookup_client(manager->handle(), client);
}

QW_END_NAMESPACE
