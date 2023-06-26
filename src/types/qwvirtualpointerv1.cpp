// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualpointerv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_virtual_pointer_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWVirtualPointerManagerV1Private : public QWObjectPrivate
{
public:
    QWVirtualPointerManagerV1Private(wlr_virtual_pointer_manager_v1 *handle, bool isOwner, QWVirtualPointerManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWVirtualPointerManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_virtual_pointer, this, &QWVirtualPointerManagerV1Private::on_new_virtual_pointer);
    }
    ~QWVirtualPointerManagerV1Private() {
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
    void on_new_virtual_pointer(wlr_virtual_pointer_v1_new_pointer_event *);

    static QHash<void*, QWVirtualPointerManagerV1*> map;
    QW_DECLARE_PUBLIC(QWVirtualPointerManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWVirtualPointerManagerV1*> QWVirtualPointerManagerV1Private::map;

void QWVirtualPointerManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWVirtualPointerManagerV1Private::on_new_virtual_pointer(wlr_virtual_pointer_v1_new_pointer_event *event)
{
    Q_EMIT q_func()->newVirtualPointer(event);
}

QWVirtualPointerManagerV1::QWVirtualPointerManagerV1(wlr_virtual_pointer_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWVirtualPointerManagerV1Private(handle, isOwner, this))
{

}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_virtual_pointer_manager_v1_create(display->handle());
    return handle ? new QWVirtualPointerManagerV1(handle, true) : nullptr;
}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::get(wlr_virtual_pointer_manager_v1 *handle)
{
    return QWVirtualPointerManagerV1Private::map.value(handle);
}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::from(wlr_virtual_pointer_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWVirtualPointerManagerV1(handle, false);
}

QW_END_NAMESPACE

