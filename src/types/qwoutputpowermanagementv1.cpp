// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputpowermanagementv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_output_power_management_v1.h>
}

QW_BEGIN_NAMESPACE

class QWOutputPowerManagerV1Private : public QWObjectPrivate
{
public:
    QWOutputPowerManagerV1Private(wlr_output_power_manager_v1 *handle, bool isOwner, QWOutputPowerManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWOutputPowerManagerV1Private::on_destroy);
        sc.connect(&handle->events.set_mode, this, &QWOutputPowerManagerV1Private::on_set_mode);
    }
    ~QWOutputPowerManagerV1Private() {
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
    void on_set_mode(void *);

    static QHash<void*, QWOutputPowerManagerV1*> map;
    QW_DECLARE_PUBLIC(QWOutputPowerManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWOutputPowerManagerV1*> QWOutputPowerManagerV1Private::map;

void QWOutputPowerManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWOutputPowerManagerV1Private::on_set_mode(void *data)
{
    Q_EMIT q_func()->modeChanged(reinterpret_cast<wlr_output_power_v1_set_mode_event*>(data));
}

QWOutputPowerManagerV1::QWOutputPowerManagerV1(wlr_output_power_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWOutputPowerManagerV1Private(handle, isOwner, this))
{

}

QWOutputPowerManagerV1 *QWOutputPowerManagerV1::get(wlr_output_power_manager_v1 *handle)
{
    return QWOutputPowerManagerV1Private::map.value(handle);
}

QWOutputPowerManagerV1 *QWOutputPowerManagerV1::from(wlr_output_power_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWOutputPowerManagerV1(handle, false);
}

QWOutputPowerManagerV1 *QWOutputPowerManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_output_power_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWOutputPowerManagerV1(handle, true);
}

QW_END_NAMESPACE
