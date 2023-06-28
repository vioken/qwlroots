// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtablet.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QWTabletToolPrivate : public QWObjectPrivate
{
public:
    QWTabletToolPrivate(wlr_tablet_tool *handle, bool isOwner, QWTabletTool *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWTabletToolPrivate::on_destroy);
    }
    ~QWTabletToolPrivate() {
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

    static QHash<void*, QWTabletTool*> map;
    QW_DECLARE_PUBLIC(QWTabletTool)
    QWSignalConnector sc;
};
QHash<void*, QWTabletTool*> QWTabletToolPrivate::map;

void QWTabletToolPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWTabletTool::QWTabletTool(wlr_tablet_tool *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWTabletToolPrivate(handle, isOwner, this))
{

}

QWTabletTool *QWTabletTool::get(wlr_tablet_tool *handle)
{
    return QWTabletToolPrivate::map.value(handle);
}

QWTabletTool *QWTabletTool::from(wlr_tablet_tool *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTabletTool(handle, false);
}

QW_END_NAMESPACE
