// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwgammacontorlv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_gamma_control_v1.h>
}

QW_BEGIN_NAMESPACE

class QWGammaControlManagerV1Private : public QWObjectPrivate
{
public:
    QWGammaControlManagerV1Private(wlr_gamma_control_manager_v1 *handle, bool isOwner, QWGammaControlManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWGammaControlManagerV1Private::on_destroy);
#if WLR_VERSION_MINOR > 16
        sc.connect(&handle->events.set_gamma, this, &QWGammaControlManagerV1Private::on_set_gamma);
#endif
    }
    ~QWGammaControlManagerV1Private() {
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
#if WLR_VERSION_MINOR > 16
    void on_set_gamma(void *);
#endif

    static QHash<void*, QWGammaControlManagerV1*> map;
    QW_DECLARE_PUBLIC(QWGammaControlManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWGammaControlManagerV1*> QWGammaControlManagerV1Private::map;

void QWGammaControlManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

#if WLR_VERSION_MINOR > 16
void QWGammaControlManagerV1Private::on_set_gamma(void *data)
{
    Q_EMIT q_func()->gammaChanged(reinterpret_cast<wlr_gamma_control_manager_v1_set_gamma_event*>(data));
}
#endif

QWGammaControlManagerV1::QWGammaControlManagerV1(wlr_gamma_control_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWGammaControlManagerV1Private(handle, isOwner, this))
{

}

QWGammaControlManagerV1 *QWGammaControlManagerV1::get(wlr_gamma_control_manager_v1 *handle)
{
    return QWGammaControlManagerV1Private::map.value(handle);
}

QWGammaControlManagerV1 *QWGammaControlManagerV1::from(wlr_gamma_control_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWGammaControlManagerV1(handle, false);
}

QWGammaControlManagerV1 *QWGammaControlManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_gamma_control_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWGammaControlManagerV1(handle, true);
}

QW_END_NAMESPACE
