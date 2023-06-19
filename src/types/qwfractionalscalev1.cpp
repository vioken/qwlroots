// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwfractionalscalev1.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_fractional_scale_v1.h>
}

QW_BEGIN_NAMESPACE

class QWFractionalScaleManagerV1Private : public QWObjectPrivate
{
public:
    QWFractionalScaleManagerV1Private(wlr_fractional_scale_manager_v1 *handle, bool isOwner, QWFractionalScaleManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWFractionalScaleManagerV1Private::on_destroy);
    }
    ~QWFractionalScaleManagerV1Private() {
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

    static QHash<void*, QWFractionalScaleManagerV1*> map;
    QW_DECLARE_PUBLIC(QWFractionalScaleManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWFractionalScaleManagerV1*> QWFractionalScaleManagerV1Private::map;

void QWFractionalScaleManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWFractionalScaleManagerV1::QWFractionalScaleManagerV1(wlr_fractional_scale_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWFractionalScaleManagerV1Private(handle, isOwner, this))
{

}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::get(wlr_fractional_scale_manager_v1 *handle)
{
    return QWFractionalScaleManagerV1Private::map.value(handle);
}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::from(wlr_fractional_scale_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWFractionalScaleManagerV1(handle, false);
}

QWFractionalScaleManagerV1 *QWFractionalScaleManagerV1::create(QWDisplay *display, uint32_t version)
{
    auto *handle = wlr_fractional_scale_manager_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWFractionalScaleManagerV1(handle, true);
}

void QWFractionalScaleManagerV1::notifyScale(QWSurface *surface, double scale)
{
    wlr_fractional_scale_v1_notify_scale(surface->handle(), scale);
}

QW_END_NAMESPACE
