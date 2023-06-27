// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputmanagementv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWOutputManagerV1Private : public QWObjectPrivate
{
public:
    QWOutputManagerV1Private(wlr_output_manager_v1 *handle, bool isOwner, QWOutputManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWOutputManagerV1Private::on_destroy);
        sc.connect(&handle->events.apply, this, &QWOutputManagerV1Private::on_apply);
        sc.connect(&handle->events.test, this, &QWOutputManagerV1Private::on_test);
    }
    ~QWOutputManagerV1Private() {
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
    void on_apply(void *);
    void on_test(void *);

    static QHash<void*, QWOutputManagerV1*> map;
    QW_DECLARE_PUBLIC(QWOutputManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWOutputManagerV1*> QWOutputManagerV1Private::map;

void QWOutputManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

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
    : QObject(nullptr)
    , QWObject(*new QWOutputManagerV1Private(handle, isOwner, this))
{

}

QWOutputManagerV1 *QWOutputManagerV1::get(wlr_output_manager_v1 *handle)
{
    return QWOutputManagerV1Private::map.value(handle);
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
