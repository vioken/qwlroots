// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwfullscreenshellv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_fullscreen_shell_v1.h>
}

QW_BEGIN_NAMESPACE

class QWFullScreenShellV1Private : public QWObjectPrivate
{
public:
    QWFullScreenShellV1Private(wlr_fullscreen_shell_v1 *handle, bool isOwner, QWFullScreenShellV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWFullScreenShellV1Private::on_destroy);
        sc.connect(&handle->events.present_surface, this, &QWFullScreenShellV1Private::on_present_surface);
    }
    ~QWFullScreenShellV1Private() {
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
    void on_present_surface(void *);

    static QHash<void*, QWFullScreenShellV1*> map;
    QW_DECLARE_PUBLIC(QWFullScreenShellV1)
    QWSignalConnector sc;
};
QHash<void*, QWFullScreenShellV1*> QWFullScreenShellV1Private::map;

void QWFullScreenShellV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWFullScreenShellV1Private::on_present_surface(void *data)
{
    Q_EMIT q_func()->presentSurface(reinterpret_cast<wlr_fullscreen_shell_v1_present_surface_event*>(data));
}

QWFullScreenShellV1::QWFullScreenShellV1(wlr_fullscreen_shell_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWFullScreenShellV1Private(handle, isOwner, this))
{

}

QWFullScreenShellV1 *QWFullScreenShellV1::get(wlr_fullscreen_shell_v1 *handle)
{
    return QWFullScreenShellV1Private::map.value(handle);
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
