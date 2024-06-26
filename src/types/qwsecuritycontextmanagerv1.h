// Copyright (C) 2024 YaoBing Xiao <xiaoyaobing@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_client;

struct wlr_security_context_v1_state;
struct wlr_security_context_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSecurityContextManagerV1Private;
class QW_EXPORT QWSecurityContextManagerV1 : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSecurityContextManagerV1)
public:
    inline wlr_security_context_manager_v1 *handle() const {
        return QWObject::handle<wlr_security_context_manager_v1>();
    }

    static QWSecurityContextManagerV1 *get(wlr_security_context_manager_v1 *handle);
    static QWSecurityContextManagerV1 *from(wlr_security_context_manager_v1 *handle);
    static QWSecurityContextManagerV1 *create(QWDisplay *display);

    static const wlr_security_context_v1_state *lookupClient(QWSecurityContextManagerV1 *manager, wl_client *client);

private:
    QWSecurityContextManagerV1(wlr_security_context_manager_v1 *handle, bool isOwner);
    ~QWSecurityContextManagerV1() = default;
};

QW_END_NAMESPACE
