// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwforeigntoplevelhandlev1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include "math.h"
#define static
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWForeignToplevelManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWForeignToplevelManagerV1Private(wlr_foreign_toplevel_manager_v1 *handle, bool isOwner, QWForeignToplevelManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWForeignToplevelManagerV1)
};

QWForeignToplevelManagerV1::QWForeignToplevelManagerV1(wlr_foreign_toplevel_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWForeignToplevelManagerV1Private(handle, isOwner, this))
{

}

QWForeignToplevelManagerV1 *QWForeignToplevelManagerV1::get(wlr_foreign_toplevel_manager_v1 *handle)
{
    return static_cast<QWForeignToplevelManagerV1*>(QWForeignToplevelManagerV1Private::map.value(handle));
}

QWForeignToplevelManagerV1 *QWForeignToplevelManagerV1::from(wlr_foreign_toplevel_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWForeignToplevelManagerV1(handle, false);
}

QWForeignToplevelManagerV1 *QWForeignToplevelManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_foreign_toplevel_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWForeignToplevelManagerV1(handle, true);
}

QW_END_NAMESPACE
