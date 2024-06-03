// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwrelativepointerv1.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_relative_pointer_v1.h>
}

QW_BEGIN_NAMESPACE

class QWRelativeV1Private : public QWWrapObjectPrivate
{
public:
    QWRelativeV1Private(wlr_relative_pointer_v1 *handle, bool isOwner, QWRelativeV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWRelativeV1)
};

QWRelativeV1::QWRelativeV1(wlr_relative_pointer_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWRelativeV1Private(handle, isOwner, this))
{

}

QWRelativeV1 *QWRelativeV1::get(wlr_relative_pointer_v1 *handle)
{
    return static_cast<QWRelativeV1*>(QWRelativeV1Private::map.value(handle));
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
