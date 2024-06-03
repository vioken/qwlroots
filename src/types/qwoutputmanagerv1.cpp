// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputmanagementv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWOutputManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWOutputManagerV1Private(wlr_output_manager_v1 *handle, bool isOwner, QWOutputManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.apply, this, &QWOutputManagerV1Private::on_apply);
        sc.connect(&handle->events.test, this, &QWOutputManagerV1Private::on_test);
    }

    void on_apply(void *);
    void on_test(void *);

    QW_DECLARE_PUBLIC(QWOutputManagerV1)
};

void QWOutputManagerV1Private::on_apply(void *data)
{
    auto *config = QWOutputConfigurationV1::from(reinterpret_cast<wlr_output_configuration_v1*>(data));
    Q_EMIT q_func()->apply(config);
}

void QWOutputManagerV1Private::on_test(void *data)
{
    auto *config = QWOutputConfigurationV1::from(reinterpret_cast<wlr_output_configuration_v1*>(data));
    Q_EMIT q_func()->test(config);
}

QWOutputManagerV1::QWOutputManagerV1(wlr_output_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWOutputManagerV1Private(handle, isOwner, this))
{

}

QWOutputManagerV1 *QWOutputManagerV1::get(wlr_output_manager_v1 *handle)
{
    return static_cast<QWOutputManagerV1*>(QWOutputManagerV1Private::map.value(handle));
}

QWOutputManagerV1 *QWOutputManagerV1::from(wlr_output_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWOutputManagerV1(handle, false);
}

QWOutputManagerV1 *QWOutputManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_output_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWOutputManagerV1(handle, true);
}

void QWOutputManagerV1::setConfiguration(QWOutputConfigurationV1 *config)
{
    wlr_output_manager_v1_set_configuration(handle(), config->handle());
}

QW_END_NAMESPACE
