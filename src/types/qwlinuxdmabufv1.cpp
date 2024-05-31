// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlinuxdmabufv1.h"
#include "qwdisplay.h"
#include "qwrenderer.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_linux_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

class QWLinuxDmabufV1Private : public QWWrapObjectPrivate
{
public:
    QWLinuxDmabufV1Private(wlr_linux_dmabuf_v1 *handle, bool isOwner, QWLinuxDmabufV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWLinuxDmabufV1)
};
QHash<void*, QWWrapObject*> QWLinuxDmabufV1Private::map;

QWLinuxDmabufV1::QWLinuxDmabufV1(wlr_linux_dmabuf_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWLinuxDmabufV1Private(handle, isOwner, this))
{

}

QWLinuxDmabufV1 *QWLinuxDmabufV1::get(wlr_linux_dmabuf_v1 *handle)
{
    return static_cast<QWLinuxDmabufV1*>(QWLinuxDmabufV1Private::map.value(handle));
}

QWLinuxDmabufV1 *QWLinuxDmabufV1::from(wlr_linux_dmabuf_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWLinuxDmabufV1(handle, false);
}

QWLinuxDmabufV1 *QWLinuxDmabufV1::create(QWDisplay *display, uint32_t version, const QWLinuxDmabufFeedbackV1 *defaultFeedback)
{
    auto *handle = wlr_linux_dmabuf_v1_create(display->handle(), version, defaultFeedback->handle());
    if (!handle)
        return nullptr;
    return new QWLinuxDmabufV1(handle, true);
}

QWLinuxDmabufV1 *QWLinuxDmabufV1::create(QWDisplay *display, uint32_t version, QWRenderer *renderer)
{
    auto *handle = wlr_linux_dmabuf_v1_create_with_renderer(display->handle(), version, renderer->handle());
    if (!handle)
        return nullptr;
    return new QWLinuxDmabufV1(handle, true);
}

QW_END_NAMESPACE
