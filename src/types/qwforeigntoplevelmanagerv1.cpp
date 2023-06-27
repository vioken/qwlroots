// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwforeigntoplevelhandlev1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include "math.h"
#define static
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWForeignToplevelManagerV1Private : public QWObjectPrivate
{
public:
    QWForeignToplevelManagerV1Private(wlr_foreign_toplevel_manager_v1 *handle, bool isOwner, QWForeignToplevelManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWForeignToplevelManagerV1Private::on_destroy);
    }
    ~QWForeignToplevelManagerV1Private() {
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

    static QHash<void*, QWForeignToplevelManagerV1*> map;
    QW_DECLARE_PUBLIC(QWForeignToplevelManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWForeignToplevelManagerV1*> QWForeignToplevelManagerV1Private::map;

void QWForeignToplevelManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWForeignToplevelManagerV1::QWForeignToplevelManagerV1(wlr_foreign_toplevel_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWForeignToplevelManagerV1Private(handle, isOwner, this))
{

}

QWForeignToplevelManagerV1 *QWForeignToplevelManagerV1::get(wlr_foreign_toplevel_manager_v1 *handle)
{
    return QWForeignToplevelManagerV1Private::map.value(handle);
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
