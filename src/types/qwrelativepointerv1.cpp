// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrelativepointerv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_relative_pointer_v1.h>
}

QW_BEGIN_NAMESPACE

class QWRelativeV1Private : public QWObjectPrivate
{
public:
    QWRelativeV1Private(wlr_relative_pointer_v1 *handle, bool isOwner, QWRelativeV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWRelativeV1Private::on_destroy);
    }
    ~QWRelativeV1Private() {
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

    static QHash<void*, QWRelativeV1*> map;
    QW_DECLARE_PUBLIC(QWRelativeV1)
    QWSignalConnector sc;
};
QHash<void*, QWRelativeV1*> QWRelativeV1Private::map;

void QWRelativeV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWRelativeV1::QWRelativeV1(wlr_relative_pointer_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWRelativeV1Private(handle, isOwner, this))
{

}

QWRelativeV1 *QWRelativeV1::get(wlr_relative_pointer_v1 *handle)
{
    return QWRelativeV1Private::map.value(handle);
}

QWRelativeV1 *QWRelativeV1::from(wlr_relative_pointer_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWRelativeV1(handle, false);
}

QWRelativeV1 *QWRelativeV1::fromResource(wl_resource *resource)
{
    auto *handle = wlr_relative_pointer_v1_from_resource(resource);
    return handle ? new QWRelativeV1(handle, true) : nullptr;
}

QW_END_NAMESPACE
