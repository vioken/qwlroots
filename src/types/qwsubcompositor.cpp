// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsubcompositor.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_subcompositor.h>
}

QW_BEGIN_NAMESPACE

class QWSubcompositorPrivate : public QWObjectPrivate
{
public:
    QWSubcompositorPrivate(wlr_subcompositor *handle, QWSubcompositor *qq)
        : QWObjectPrivate(handle, qq)
    {
        sc.connect(&handle->events.destroy, this, &QWSubcompositorPrivate::on_destroy);
    }
    ~QWSubcompositorPrivate() {
        sc.invalidate();
    }

    void on_destroy(void *);

    QW_DECLARE_PUBLIC(QWSubcompositor)
    QWSignalConnector sc;
};

void QWSubcompositorPrivate::on_destroy(void *)
{
    m_handle = nullptr;
    delete q_func();
}

QWSubcompositor::QWSubcompositor(wlr_subcompositor *handle)
    : QWObject(*new QWSubcompositorPrivate(handle, this))
{

}

QWSubcompositor *QWSubcompositor::create(wl_display *display)
{
    auto subcompositor = wlr_subcompositor_create(display);
    if (!subcompositor)
        return nullptr;
    return new QWSubcompositor(subcompositor);
}

QW_END_NAMESPACE
