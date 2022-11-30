// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcompositor.h"
#include "util/qwsignalconnector.h"
#include "render/qwrenderer.h"

extern "C" {
#include <wlr/types/wlr_compositor.h>
}

QW_BEGIN_NAMESPACE

class QWCompositorPrivate : public QWObjectPrivate
{
public:
    QWCompositorPrivate(wlr_compositor *handle, QWCompositor *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&handle->events.destroy, this, &QWCompositorPrivate::on_destroy);
        sc.connect(&handle->events.new_surface, this, &QWCompositorPrivate::on_new_surface);
    }
    ~QWCompositorPrivate() {
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_surface(void *data);

    QW_DECLARE_PUBLIC(QWCompositor)
    QWSignalConnector sc;
};

void QWCompositorPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    q_func()->deleteLater();
}

void QWCompositorPrivate::on_new_surface(void *data)
{
    Q_EMIT q_func()->newSurface(reinterpret_cast<wlr_surface*>(data));
}

QWCompositor::QWCompositor(wlr_compositor *handle)
    : QObject(nullptr)
    , QWObject(*new QWCompositorPrivate(handle, this))
{

}

QWCompositor *QWCompositor::create(wl_display *display, QWRenderer *renderer)
{
    auto compositor = wlr_compositor_create(display, renderer->handle());
    if (!compositor)
        return nullptr;
    return new QWCompositor(compositor);
}

QW_END_NAMESPACE
