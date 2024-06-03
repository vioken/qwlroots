// Copyright (C) 2024 YaoBing Xiao <xiaoyaobing@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsecuritycontextmanagerv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_security_context_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSecurityContextManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWSecurityContextManagerV1Private(wlr_security_context_manager_v1 *handle, bool isOwner, QWSecurityContextManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWSecurityContextManagerV1)
};

QWSecurityContextManagerV1::QWSecurityContextManagerV1(wlr_security_context_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWSecurityContextManagerV1Private(handle, isOwner, this))
{

}

QWSecurityContextManagerV1 *QWSecurityContextManagerV1::get(wlr_security_context_manager_v1 *handle)
{
    return static_cast<QWSecurityContextManagerV1*>(QWSecurityContextManagerV1Private::map.value(handle));
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
