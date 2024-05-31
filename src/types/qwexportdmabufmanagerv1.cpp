// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwexportdmabufv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_export_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

class QWExportDmabufManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWExportDmabufManagerV1Private(wlr_export_dmabuf_manager_v1 *handle, bool isOwner, QWExportDmabufManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWExportDmabufManagerV1)
};
QHash<void*, QWWrapObject*> QWExportDmabufManagerV1Private::map;

QWExportDmabufManagerV1::QWExportDmabufManagerV1(wlr_export_dmabuf_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWExportDmabufManagerV1Private(handle, isOwner, this))
{

}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::get(wlr_export_dmabuf_manager_v1 *handle)
{
    return static_cast<QWExportDmabufManagerV1*>(QWExportDmabufManagerV1Private::map.value(handle));
}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::from(wlr_export_dmabuf_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWExportDmabufManagerV1(handle, false);
}

QWExportDmabufManagerV1 *QWExportDmabufManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_export_dmabuf_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWExportDmabufManagerV1(handle, true);
}

QW_END_NAMESPACE
