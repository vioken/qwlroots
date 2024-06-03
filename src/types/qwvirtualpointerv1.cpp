// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualpointerv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_virtual_pointer_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWVirtualPointerManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWVirtualPointerManagerV1Private(wlr_virtual_pointer_manager_v1 *handle, bool isOwner, QWVirtualPointerManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_virtual_pointer, this, &QWVirtualPointerManagerV1Private::on_new_virtual_pointer);
    }

    void on_new_virtual_pointer(wlr_virtual_pointer_v1_new_pointer_event *);

    QW_DECLARE_PUBLIC(QWVirtualPointerManagerV1)
};

void QWVirtualPointerManagerV1Private::on_new_virtual_pointer(wlr_virtual_pointer_v1_new_pointer_event *event)
{
    Q_EMIT q_func()->newVirtualPointer(event);
}

QWVirtualPointerManagerV1::QWVirtualPointerManagerV1(wlr_virtual_pointer_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWVirtualPointerManagerV1Private(handle, isOwner, this))
{

}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_virtual_pointer_manager_v1_create(display->handle());
    return handle ? new QWVirtualPointerManagerV1(handle, true) : nullptr;
}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::get(wlr_virtual_pointer_manager_v1 *handle)
{
    return static_cast<QWVirtualPointerManagerV1*>(QWVirtualPointerManagerV1Private::map.value(handle));
}

QWVirtualPointerManagerV1 *QWVirtualPointerManagerV1::from(wlr_virtual_pointer_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWVirtualPointerManagerV1(handle, false);
}

QW_END_NAMESPACE

