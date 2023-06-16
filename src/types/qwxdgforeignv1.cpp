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
class QWXdgForeignV1Private : public QWObjectPrivate
{
public:
    QWXdgForeignV1Private(wlr_xdg_foreign_v1 *handle, bool isOwner, QWXdgForeignV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgForeignV1Private::on_destroy);
    }

    ~QWXdgForeignV1Private() override {
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

    static QHash<void*, QWXdgForeignV1*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgForeignV1*> QWXdgForeignV1Private::map;

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
    return QWXdgForeignV1Private::map.value(handle);
}

QWXdgForeignV1::QWXdgForeignV1(wlr_xdg_foreign_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(* new QWXdgForeignV1Private(handle, isOwner, this))
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
