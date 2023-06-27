// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdrmleasev1.h"
#include "qwdisplay.h"
#include "qwbackend.h"
#include "qwoutput.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_drm_lease_v1.h>
}

QW_BEGIN_NAMESPACE

class QWDrmLeaseV1ManagerPrivate : public QWObjectPrivate
{
public:
    QWDrmLeaseV1ManagerPrivate(wlr_drm_lease_v1_manager *handle, bool isOwner, QWDrmLeaseV1Manager *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.request, this, &QWDrmLeaseV1ManagerPrivate::on_request);
    }
    ~QWDrmLeaseV1ManagerPrivate() {
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

    void on_request(void *);

    static QHash<void*, QWDrmLeaseV1Manager*> map;
    QW_DECLARE_PUBLIC(QWDrmLeaseV1Manager)
    QWSignalConnector sc;
};
QHash<void*, QWDrmLeaseV1Manager*> QWDrmLeaseV1ManagerPrivate::map;

void QWDrmLeaseV1ManagerPrivate::on_request(void *data)
{
    auto *drmLeaseRequest = QWDrmLeaseRequestV1::from(reinterpret_cast<wlr_drm_lease_request_v1*>(data));
    Q_EMIT q_func()->request(drmLeaseRequest);
}

QWDrmLeaseV1Manager::QWDrmLeaseV1Manager(wlr_drm_lease_v1_manager *handle, bool isOwner, QWDisplay *parent)
    : QObject(parent)
    , QWObject(*new QWDrmLeaseV1ManagerPrivate(handle, isOwner, this))
{

}

QWDrmLeaseV1Manager *QWDrmLeaseV1Manager::get(wlr_drm_lease_v1_manager *handle)
{
    return QWDrmLeaseV1ManagerPrivate::map.value(handle);
}

QWDrmLeaseV1Manager *QWDrmLeaseV1Manager::create(QWDisplay *display, QWBackend *backend)
{
    auto *handle = wlr_drm_lease_v1_manager_create(display->handle(), backend->handle());
    if (!handle)
        return nullptr;
    return new QWDrmLeaseV1Manager(handle, true, display);
}

bool QWDrmLeaseV1Manager::offerOutput(QWOutput *output)
{
    return wlr_drm_lease_v1_manager_offer_output(handle(), output->handle());
}

void QWDrmLeaseV1Manager::withdrawOutput(QWOutput *output)
{
    wlr_drm_lease_v1_manager_withdraw_output(handle(), output->handle());
}

QW_END_NAMESPACE
