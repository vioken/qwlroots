// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcompositor.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"
#include "render/qwrenderer.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_compositor.h>
}

QW_BEGIN_NAMESPACE

class QWCompositorPrivate : public QWObjectPrivate
{
public:
    QWCompositorPrivate(wlr_compositor *handle, bool isOwner, QWCompositor *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWCompositorPrivate::on_destroy);
        sc.connect(&handle->events.new_surface, this, &QWCompositorPrivate::on_new_surface);
    }
    ~QWCompositorPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            qFatal("QWCompositor(%p) can't to destroy, its ownership is wl_display", q_func());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(void *data);

    static QHash<void*, QWCompositor*> map;
    QW_DECLARE_PUBLIC(QWCompositor)
    QWSignalConnector sc;
};
QHash<void*, QWCompositor*> QWCompositorPrivate::map;

void QWCompositorPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWCompositorPrivate::on_new_surface(void *data)
{
    Q_EMIT q_func()->newSurface(reinterpret_cast<wlr_surface*>(data));
}

QWCompositor::QWCompositor(wlr_compositor *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWCompositorPrivate(handle, isOwner, this))
{

}

QWCompositor *QWCompositor::get(wlr_compositor *handle)
{
    return QWCompositorPrivate::map.value(handle);
}

QWCompositor *QWCompositor::from(wlr_compositor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWCompositor(handle, false);
}

QWCompositor *QWCompositor::create(QWDisplay *display, QWRenderer *renderer)
{
    auto compositor = wlr_compositor_create(display->handle(), renderer->handle());
    if (!compositor)
        return nullptr;
    return new QWCompositor(compositor, true);
}

QW_END_NAMESPACE
