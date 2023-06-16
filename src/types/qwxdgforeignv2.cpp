// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgforeignv2.h"

#include <qwsignalconnector.h>
#include <qwdisplay.h>
#include <qwxdgforeignregistry_p.h>

#include <QHash>

extern "C" {
#include <wlr/types/wlr_xdg_foreign_v2.h>
}

QW_BEGIN_NAMESPACE
class QWXdgForeignv2Private : public QWObjectPrivate
{
public:
    QWXdgForeignv2Private(wlr_xdg_foreign_v2 *handle, bool isOwner, QWXdgForeignv2 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgForeignv2Private::on_destroy);
    }

    ~QWXdgForeignv2Private() override {
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

    static QHash<void*, QWXdgForeignv2*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignv2)
    QWSignalConnector sc;
};
QHash<void*, QWXdgForeignv2*> QWXdgForeignv2Private::map;

class QWXdgExportedv2Private : public QWXdgForeignExportedPrivate
{
public:
    QWXdgExportedv2Private(wlr_xdg_exported_v2 *handle, bool isOwner, QWXdgExportedv2 *qq)
        : QWXdgForeignExportedPrivate(&handle->base, isOwner, qq)
    {}

    ~QWXdgExportedv2Private() = default;

    QW_DECLARE_PUBLIC(QWXdgExportedv2)
};

QWXdgForeignv2* QWXdgForeignv2::create(QWDisplay *display, QWXdgForeignRegistry *registry)
{
    auto *handle = wlr_xdg_foreign_v2_create(display->handle(), registry->handle());
    return handle ? new QWXdgForeignv2(handle, true) : nullptr;
}

QWXdgForeignv2* QWXdgForeignv2::from(wlr_xdg_foreign_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgForeignv2(handle, false);
}

QWXdgForeignv2* QWXdgForeignv2::get(wlr_xdg_foreign_v2 *handle)
{
    return QWXdgForeignv2Private::map.value(handle);
}

QWXdgForeignv2::QWXdgForeignv2(wlr_xdg_foreign_v2 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(* new QWXdgForeignv2Private(handle, isOwner, this))
{
}

QWXdgExportedv2 *QWXdgExportedv2::from(wlr_xdg_exported_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgExportedv2(handle, false);
}

QWXdgExportedv2 *QWXdgExportedv2::get(wlr_xdg_exported_v2 *handle)
{
    return qobject_cast<QWXdgExportedv2*>(QWXdgForeignExported::get(&handle->base));
}

QWXdgExportedv2::QWXdgExportedv2(wlr_xdg_exported_v2 *handle, bool isOwner)
    : QWXdgForeignExported(*new QWXdgExportedv2Private(handle, isOwner, this))
{
}

QW_END_NAMESPACE
