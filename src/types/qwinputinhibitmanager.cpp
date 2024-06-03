// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputinhibitmanager.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>

#include <QHash>

extern "C" {
#include <wlr/types/wlr_input_inhibitor.h>
}

QW_BEGIN_NAMESPACE

class QWInputInhibitManagerPrivate : public QWWrapObjectPrivate
{
public:
    QWInputInhibitManagerPrivate(wlr_input_inhibit_manager *handle, bool isOwner, QWInputInhibitManager *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.activate, this, &QWInputInhibitManagerPrivate::on_activate);
        sc.connect(&handle->events.deactivate, this, &QWInputInhibitManagerPrivate::on_deactivate);
    }

    void on_activate(void *);
    void on_deactivate(void *);

    QW_DECLARE_PUBLIC(QWInputInhibitManager)
};

void QWInputInhibitManagerPrivate::on_activate(void *)
{
    // self
    Q_EMIT q_func()->activate();
}

void QWInputInhibitManagerPrivate::on_deactivate(void *)
{
    // self
    Q_EMIT q_func()->deactivate();
}

QWInputInhibitManager::QWInputInhibitManager(wlr_input_inhibit_manager *handle, bool isOwner)
    : QWWrapObject(*new QWInputInhibitManagerPrivate(handle, isOwner, this))
{

}

QWInputInhibitManager *QWInputInhibitManager::get(wlr_input_inhibit_manager *handle)
{
    return static_cast<QWInputInhibitManager*>(QWInputInhibitManagerPrivate::map.value(handle));
}

 QWInputInhibitManager *QWInputInhibitManager::from(wlr_input_inhibit_manager *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputInhibitManager(handle, false);
}

QWInputInhibitManager *QWInputInhibitManager::create(QWDisplay *display)
{
    auto *handle = wlr_input_inhibit_manager_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWInputInhibitManager(handle, true);
}

QW_END_NAMESPACE
