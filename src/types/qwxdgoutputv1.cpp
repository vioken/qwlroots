// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgoutputv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_output_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWXdgOutputManagerV1Private : public QWObjectPrivate
{
public:
    QWXdgOutputManagerV1Private(wlr_xdg_output_manager_v1 *handle, bool isOwner, QWXdgOutputManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgOutputManagerV1Private::on_destroy);
    }
    ~QWXdgOutputManagerV1Private() {
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

    static QHash<void*, QWXdgOutputManagerV1*> map;
    QW_DECLARE_PUBLIC(QWXdgOutputManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgOutputManagerV1*> QWXdgOutputManagerV1Private::map;

void QWXdgOutputManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWXdgOutputManagerV1::QWXdgOutputManagerV1(wlr_xdg_output_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgOutputManagerV1Private(handle, isOwner, this))
{

}

QWXdgOutputManagerV1 *QWXdgOutputManagerV1::get(wlr_xdg_output_manager_v1 *handle)
{
    return QWXdgOutputManagerV1Private::map.value(handle);
}

QWXdgOutputManagerV1 *QWXdgOutputManagerV1::from(wlr_xdg_output_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgOutputManagerV1(handle, false);
}

QW_END_NAMESPACE

