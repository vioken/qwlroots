// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgforeignv1.h"

#include <qwsignalconnector.h>
#include <qwdisplay.h>
#include <qwxdgforeignregistry_p.h>

#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_foreign_v1.h>
}

QW_BEGIN_NAMESPACE
class QWXdgForeignV1Private : public QWWrapObjectPrivate
{
public:
    QWXdgForeignV1Private(wlr_xdg_foreign_v1 *handle, bool isOwner, QWXdgForeignV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignV1)
};
QHash<void*, QWWrapObject*> QWXdgForeignV1Private::map;

class QWXdgExportedV1Private : public QWXdgForeignExportedPrivate
{
public:
    QWXdgExportedV1Private(wlr_xdg_exported_v1 *handle, bool isOwner, QWXdgExportedV1 *qq)
        : QWXdgForeignExportedPrivate(&handle->base, isOwner, qq)
    {}

    ~QWXdgExportedV1Private() = default;

    QW_DECLARE_PUBLIC(QWXdgExportedV1)
};

QWXdgForeignV1* QWXdgForeignV1::create(QWDisplay *display, QWXdgForeignRegistry *registry)
{
    auto *handle = wlr_xdg_foreign_v1_create(display->handle(), registry->handle());
    return handle ? new QWXdgForeignV1(handle, true) : nullptr;
}

QWXdgForeignV1* QWXdgForeignV1::from(wlr_xdg_foreign_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgForeignV1(handle, false);
}

QWXdgForeignV1* QWXdgForeignV1::get(wlr_xdg_foreign_v1 *handle)
{
    return static_cast<QWXdgForeignV1*>(QWXdgForeignV1Private::map.value(handle));
}

QWXdgForeignV1::QWXdgForeignV1(wlr_xdg_foreign_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWXdgForeignV1Private(handle, isOwner, this))
{
}

QWXdgExportedV1 *QWXdgExportedV1::from(wlr_xdg_exported_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgExportedV1(handle, false);
}

QWXdgExportedV1 *QWXdgExportedV1::get(wlr_xdg_exported_v1 *handle)
{
    return qobject_cast<QWXdgExportedV1*>(QWXdgForeignExported::get(&handle->base));
}

QWXdgExportedV1::QWXdgExportedV1(wlr_xdg_exported_v1 *handle, bool isOwner)
    : QWXdgForeignExported(*new QWXdgExportedV1Private(handle, isOwner, this))
{
}

QW_END_NAMESPACE
