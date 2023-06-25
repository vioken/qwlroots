// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatacontrolv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_control_v1.h>
}

QW_BEGIN_NAMESPACE

class QWDataControlManagerV1Private : public QWObjectPrivate
{
public:
    QWDataControlManagerV1Private(wlr_data_control_manager_v1 *handle, bool isOwner, QWDataControlManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWDataControlManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_device, this, &QWDataControlManagerV1Private::on_new_device);
    }
    ~QWDataControlManagerV1Private() {
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
    void on_new_device(void *);

    static QHash<void*, QWDataControlManagerV1*> map;
    QW_DECLARE_PUBLIC(QWDataControlManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWDataControlManagerV1*> QWDataControlManagerV1Private::map;

void QWDataControlManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWDataControlManagerV1Private::on_new_device(void *data)
{
    auto *device = QWDataControlDeviceV1::from(reinterpret_cast<wlr_data_control_device_v1*>(data));
    Q_EMIT q_func()->newDevice(device);
}

QWDataControlManagerV1::QWDataControlManagerV1(wlr_data_control_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWDataControlManagerV1Private(handle, isOwner, this))
{

}

QWDataControlManagerV1 *QWDataControlManagerV1::get(wlr_data_control_manager_v1 *handle)
{
    return QWDataControlManagerV1Private::map.value(handle);
}

QWDataControlManagerV1 *QWDataControlManagerV1::from(wlr_data_control_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDataControlManagerV1(handle, false);
}

QWDataControlManagerV1 *QWDataControlManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_data_control_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWDataControlManagerV1(handle, true);
}

QW_END_NAMESPACE
