// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtablet.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QWTabletToolPrivate : public QWWrapObjectPrivate
{
public:
    QWTabletToolPrivate(wlr_tablet_tool *handle, bool isOwner, QWTabletTool *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWTabletTool)
};

QWTabletTool::QWTabletTool(wlr_tablet_tool *handle, bool isOwner)
    : QWWrapObject(*new QWTabletToolPrivate(handle, isOwner, this))
{

}

QWTabletTool *QWTabletTool::get(wlr_tablet_tool *handle)
{
    return static_cast<QWTabletTool*>(QWTabletToolPrivate::map.value(handle));
}

QWTabletTool *QWTabletTool::from(wlr_tablet_tool *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTabletTool(handle, false);
}

QW_END_NAMESPACE
