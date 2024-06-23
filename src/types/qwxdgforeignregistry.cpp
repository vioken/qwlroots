// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgforeignregistry.h"
#include "private/qwglobal_p.h"

#include <qw_global.h>
#include <qwxdgforeignregistry_p.h>
#include <qwdisplay.h>

extern "C" {
#include <wlr/types/wlr_xdg_foreign_registry.h>
}

QW_BEGIN_NAMESPACE

class QWXdgForeignRegistryPrivate : public QWWrapObjectPrivate
{
public:
    QWXdgForeignRegistryPrivate(wlr_xdg_foreign_registry *handle, bool isOwner, QWXdgForeignRegistry *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWXdgForeignRegistry)
};

QWXdgForeignExportedPrivate::QWXdgForeignExportedPrivate(wlr_xdg_foreign_exported *handle, bool isOwner, QWXdgForeignExported *qq)
    : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
{

}


QWXdgForeignRegistry *QWXdgForeignRegistry::create(QWDisplay *display)
{
    auto *handle = wlr_xdg_foreign_registry_create(display->handle());
    return handle ? new QWXdgForeignRegistry(handle, true) : nullptr;
}

QWXdgForeignRegistry *QWXdgForeignRegistry::from(wlr_xdg_foreign_registry *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgForeignRegistry(handle, false);
}

QWXdgForeignRegistry *QWXdgForeignRegistry::get(wlr_xdg_foreign_registry *handle)
{
    return static_cast<QWXdgForeignRegistry*>(QWXdgForeignRegistryPrivate::map.value(handle));
}

QWXdgForeignExported *QWXdgForeignRegistry::findByHandle(const char *handle)
{
    auto *exported = wlr_xdg_foreign_registry_find_by_handle(this->handle(), handle);
    return exported ? QWXdgForeignExported::from(exported) : nullptr;
}

QWXdgForeignRegistry::QWXdgForeignRegistry(wlr_xdg_foreign_registry *handle, bool isOwner)
    : QWWrapObject(*new QWXdgForeignRegistryPrivate(handle, isOwner, this))
{
}

QWXdgForeignExported *QWXdgForeignExported::from(wlr_xdg_foreign_exported *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgForeignExported(handle, false);
}

QWXdgForeignExported *QWXdgForeignExported::get(wlr_xdg_foreign_exported *handle)
{
    return static_cast<QWXdgForeignExported*>(QWXdgForeignExportedPrivate::map.value(handle));
}

bool QWXdgForeignExported::init(QWXdgForeignRegistry *registry)
{
    return wlr_xdg_foreign_exported_init(handle(), registry->handle());
}

void QWXdgForeignExported::finish()
{
    return wlr_xdg_foreign_exported_finish(handle());
}

QWXdgForeignExported::QWXdgForeignExported(QWXdgForeignExportedPrivate &dd)
    : QWWrapObject(dd)
{
}

QWXdgForeignExported::QWXdgForeignExported(wlr_xdg_foreign_exported *handle, bool isOwner)
    : QWWrapObject(*new QWXdgForeignExportedPrivate(handle, isOwner, this))
{
}

QW_END_NAMESPACE
