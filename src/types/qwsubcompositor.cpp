// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsubcompositor.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_subcompositor.h>
}

QW_BEGIN_NAMESPACE

class QWSubcompositorPrivate : public QWObjectPrivate
{
public:
    QWSubcompositorPrivate(wlr_subcompositor *handle, bool isOwner, QWSubcompositor *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWSubcompositorPrivate::on_destroy);
    }
    ~QWSubcompositorPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWSubcompositor(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWSubcompositor*> map;
    QW_DECLARE_PUBLIC(QWSubcompositor)
    QWSignalConnector sc;
};
QHash<void*, QWSubcompositor*> QWSubcompositorPrivate::map;

void QWSubcompositorPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWSubcompositor::QWSubcompositor(wlr_subcompositor *handle, bool isOwner)
    : QWObject(*new QWSubcompositorPrivate(handle, isOwner, this))
{

}

QWSubcompositor *QWSubcompositor::get(wlr_subcompositor *handle)
{
    return QWSubcompositorPrivate::map.value(handle);
}

QWSubcompositor *QWSubcompositor::from(wlr_subcompositor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSubcompositor(handle, false);
}

QWSubcompositor *QWSubcompositor::create(QWDisplay *display)
{
    auto subcompositor = wlr_subcompositor_create(display->handle());
    if (!subcompositor)
        return nullptr;
    return new QWSubcompositor(subcompositor, true);
}

QW_END_NAMESPACE
