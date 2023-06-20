// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsinglepixelbufferv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_single_pixel_buffer_v1.h>
}

QW_BEGIN_NAMESPACE

class QWSinglePixelBufferManagerV1Private : public QWObjectPrivate
{
public:
    QWSinglePixelBufferManagerV1Private(wlr_single_pixel_buffer_manager_v1 *handle, bool isOwner, QWSinglePixelBufferManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
    }
    ~QWSinglePixelBufferManagerV1Private() {
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

    static QHash<void*, QWSinglePixelBufferManagerV1*> map;
    QW_DECLARE_PUBLIC(QWSinglePixelBufferManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWSinglePixelBufferManagerV1*> QWSinglePixelBufferManagerV1Private::map;

QWSinglePixelBufferManagerV1::QWSinglePixelBufferManagerV1(wlr_single_pixel_buffer_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSinglePixelBufferManagerV1Private(handle, isOwner, this))
{

}

QWSinglePixelBufferManagerV1 *QWSinglePixelBufferManagerV1::get(wlr_single_pixel_buffer_manager_v1 *handle)
{
    return QWSinglePixelBufferManagerV1Private::map.value(handle);
}

QWSinglePixelBufferManagerV1 *QWSinglePixelBufferManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_single_pixel_buffer_manager_v1_create(display->handle());
    return handle ? new QWSinglePixelBufferManagerV1(handle, true) : nullptr;
}

QWSinglePixelBufferManagerV1 *QWSinglePixelBufferManagerV1::from(wlr_single_pixel_buffer_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSinglePixelBufferManagerV1(handle, false);
}

QW_END_NAMESPACE

