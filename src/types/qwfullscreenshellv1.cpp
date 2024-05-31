// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwfullscreenshellv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_fullscreen_shell_v1.h>
}

QW_BEGIN_NAMESPACE

class QWFullScreenShellV1Private : public QWWrapObjectPrivate
{
public:
    QWFullScreenShellV1Private(wlr_fullscreen_shell_v1 *handle, bool isOwner, QWFullScreenShellV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.present_surface, this, &QWFullScreenShellV1Private::on_present_surface);
    }

    void on_present_surface(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWFullScreenShellV1)
};
QHash<void*, QWWrapObject*> QWFullScreenShellV1Private::map;

void QWFullScreenShellV1Private::on_present_surface(void *data)
{
    Q_EMIT q_func()->presentSurface(reinterpret_cast<wlr_fullscreen_shell_v1_present_surface_event*>(data));
}

QWFullScreenShellV1::QWFullScreenShellV1(wlr_fullscreen_shell_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWFullScreenShellV1Private(handle, isOwner, this))
{

}

QWFullScreenShellV1 *QWFullScreenShellV1::get(wlr_fullscreen_shell_v1 *handle)
{
    return static_cast<QWFullScreenShellV1*>(QWFullScreenShellV1Private::map.value(handle));
}

QWFullScreenShellV1 *QWFullScreenShellV1::from(wlr_fullscreen_shell_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWFullScreenShellV1(handle, false);
}

QWFullScreenShellV1 *QWFullScreenShellV1::create(QWDisplay *display)
{
    auto *handle = wlr_fullscreen_shell_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWFullScreenShellV1(handle, true);
}

QW_END_NAMESPACE
