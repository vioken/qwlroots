// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwscreencopyv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_screencopy_v1.h>
}

QW_BEGIN_NAMESPACE

class QWScreenCopyManagerV1Private : public QWObjectPrivate
{
public:
    QWScreenCopyManagerV1Private(wlr_screencopy_manager_v1 *handle, bool isOwner, QWScreenCopyManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWScreenCopyManagerV1Private::on_destroy);
    }
    ~QWScreenCopyManagerV1Private() {
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

    static QHash<void*, QWScreenCopyManagerV1*> map;
    QW_DECLARE_PUBLIC(QWScreenCopyManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWScreenCopyManagerV1*> QWScreenCopyManagerV1Private::map;

void QWScreenCopyManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWScreenCopyManagerV1::QWScreenCopyManagerV1(wlr_screencopy_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWScreenCopyManagerV1Private(handle, isOwner, this))
{

}

QWScreenCopyManagerV1 *QWScreenCopyManagerV1::get(wlr_screencopy_manager_v1 *handle)
{
    return QWScreenCopyManagerV1Private::map.value(handle);
}

QWScreenCopyManagerV1 *QWScreenCopyManagerV1::from(wlr_screencopy_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWScreenCopyManagerV1(handle, false);
}

QWScreenCopyManagerV1 *QWScreenCopyManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_screencopy_manager_v1_create(display->handle());
    return handle ? new QWScreenCopyManagerV1(handle, true) : nullptr;
}

QW_END_NAMESPACE
