// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_drm_lease_v1_manager;
struct wlr_drm_lease_request_v1;
struct wlr_drm_lease_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWBackend;
class QWOutput;

class QW_EXPORT QWDrmLeaseV1
{
public:
    QWDrmLeaseV1() = delete;
    ~QWDrmLeaseV1() = delete;

    wlr_drm_lease_v1 *handle() const;

    static QWDrmLeaseV1 *from(wlr_drm_lease_v1 *handle);
};

class QW_EXPORT QWDrmLeaseRequestV1
{
public:
    QWDrmLeaseRequestV1() = delete;
    ~QWDrmLeaseRequestV1() = delete;

    wlr_drm_lease_request_v1 *handle() const;

    static QWDrmLeaseRequestV1 *from(wlr_drm_lease_request_v1 *handle);

    QWDrmLeaseV1* grant();
    void reject();
};

class QWDrmLeaseV1ManagerPrivate;
class QW_EXPORT QWDrmLeaseV1Manager : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDrmLeaseV1Manager)
public:
    ~QWDrmLeaseV1Manager() = default;
    explicit QWDrmLeaseV1Manager(wlr_drm_lease_v1_manager *handle, bool isOwner, QWDisplay *parent);

    inline wlr_drm_lease_v1_manager *handle() const {
        return QWObject::handle<wlr_drm_lease_v1_manager>();
    }

    static QWDrmLeaseV1Manager *get(wlr_drm_lease_v1_manager *handle);
    static QWDrmLeaseV1Manager *create(QWDisplay *display, QWBackend *backend);

    bool offerOutput(QWOutput *output);
    void withdrawOutput(QWOutput *output);

Q_SIGNALS:
    void beforeDestroy(QWDrmLeaseV1Manager *self);
    void request(QWDrmLeaseRequestV1 *request);
};

QW_END_NAMESPACE
