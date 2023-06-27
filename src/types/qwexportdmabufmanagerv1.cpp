// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwexportdmabufv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_export_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

class QWExportDmabufManagerV1Private : public QWObjectPrivate
{
public:
    QWExportDmabufManagerV1Private(wlr_export_dmabuf_manager_v1 *handle, bool isOwner, QWExportDmabufManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWExportDmabufManagerV1Private::on_destroy);
    }
    ~QWExportDmabufManagerV1Private() {
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

    static QHash<void*, QWExportDmabufManagerV1*> map;
    QW_DECLARE_PUBLIC(QWExportDmabufManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWExportDmabufManagerV1*> QWExportDmabufManagerV1Private::map;

void QWExportDmabufManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWExportDmabufManagerV1::QWExportDmabufManagerV1(wlr_export_dmabuf_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWExportDmabufManagerV1Private(handle, isOwner, this))
{

}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::get(wlr_export_dmabuf_manager_v1 *handle)
{
    return QWExportDmabufManagerV1Private::map.value(handle);
}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::from(wlr_export_dmabuf_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWExportDmabufManagerV1(handle, false);
}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_export_dmabuf_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWExportDmabufManagerV1(handle, true);
}

QW_END_NAMESPACE
