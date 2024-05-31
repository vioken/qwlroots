// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgoutputv1.h"
#include "qwdisplay.h"
#include "qwoutputlayout.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_output_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWXdgOutputManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWXdgOutputManagerV1Private(wlr_xdg_output_manager_v1 *handle, bool isOwner, QWXdgOutputManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXdgOutputManagerV1)
};
QHash<void*, QWWrapObject*> QWXdgOutputManagerV1Private::map;

QWXdgOutputManagerV1::QWXdgOutputManagerV1(wlr_xdg_output_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWXdgOutputManagerV1Private(handle, isOwner, this))
{

}

QWXdgOutputManagerV1 *QWXdgOutputManagerV1::get(wlr_xdg_output_manager_v1 *handle)
{
    return static_cast<QWXdgOutputManagerV1*>(QWXdgOutputManagerV1Private::map.value(handle));
}

QWXdgOutputManagerV1 *QWXdgOutputManagerV1::from(wlr_xdg_output_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgOutputManagerV1(handle, false);
}

QWXdgOutputManagerV1 *QWXdgOutputManagerV1::create(QWDisplay *display, QWOutputLayout *layout)
{
    auto *handle = wlr_xdg_output_manager_v1_create(display->handle(), layout->handle());
    if (!handle)
        return nullptr;
    return new QWXdgOutputManagerV1(handle, true);
}

QW_END_NAMESPACE

