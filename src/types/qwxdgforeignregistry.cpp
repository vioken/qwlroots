// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgforeignregistry.h"

#include <qwglobal.h>
#include <qwxdgforeignregistry_p.h>
#include <qwdisplay.h>

extern "C" {
#include <wlr/types/wlr_xdg_foreign_registry.h>
}

QW_BEGIN_NAMESPACE

class QWXdgForeignRegistryPrivate : public QWObjectPrivate
{
public:
    QWXdgForeignRegistryPrivate(wlr_xdg_foreign_registry *handle, bool isOwner, QWXdgForeignRegistry *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgForeignRegistryPrivate::on_destroy);
    }


    ~QWXdgForeignRegistryPrivate() override {
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

    inline void on_destroy() {
        destroy();
        m_handle = nullptr;
        delete q_func();
    }

    static QHash<void*, QWXdgForeignRegistry*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignRegistry)
    QWSignalConnector sc;
};
QHash<void*, QWXdgForeignRegistry*> QWXdgForeignRegistryPrivate::map;

QWXdgForeignExportedPrivate::QWXdgForeignExportedPrivate(wlr_xdg_foreign_exported *handle, bool isOwner, QWXdgForeignExported *qq)
    : QWObjectPrivate(handle, isOwner, qq)
{
    Q_ASSERT(!map.contains(handle));
    map.insert(handle, qq);
    sc.connect(&handle->events.destroy, this, &QWXdgForeignExportedPrivate::on_destroy);
}

QWXdgForeignExportedPrivate::~QWXdgForeignExportedPrivate()
{
    if (!m_handle)
        return;
    destroy();
}

void QWXdgForeignExportedPrivate::destroy() {
    Q_ASSERT(m_handle);
    Q_ASSERT(map.contains(m_handle));
    Q_EMIT q_func()->beforeDestroy(q_func());
    map.remove(m_handle);
    sc.invalidate();
}

void QWXdgForeignExportedPrivate::on_destroy()
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QHash<void*, QWXdgForeignExported*> QWXdgForeignExportedPrivate::map;

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
    return QWXdgForeignRegistryPrivate::map.value(handle);
}

QWXdgForeignExported *QWXdgForeignRegistry::findByHandle(const char *handle)
{
    auto *exported = wlr_xdg_foreign_registry_find_by_handle(this->handle(), handle);
    return exported ? QWXdgForeignExported::from(exported) : nullptr;
}

QWXdgForeignRegistry::QWXdgForeignRegistry(wlr_xdg_foreign_registry *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(* new QWXdgForeignRegistryPrivate(handle, isOwner, this))
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
    return QWXdgForeignExportedPrivate::map.value(handle);
}

bool QWXdgForeignExported::init(QWXdgForeignRegistry *registry)
{
    return wlr_xdg_foreign_exported_init(handle(), registry->handle());
}

void QWXdgForeignExported::finish()
{
    return wlr_xdg_foreign_exported_finish(handle());
}

QWXdgForeignExported::QWXdgForeignExported(QWObjectPrivate &dd)
    : QObject(nullptr)
    , QWObject(dd)
{
}

QWXdgForeignExported::QWXdgForeignExported(wlr_xdg_foreign_exported *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(* new QWXdgForeignExportedPrivate(handle, isOwner, this))
{
}

QW_END_NAMESPACE
