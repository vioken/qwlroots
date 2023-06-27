// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlinuxdmabufv1.h"
#include "qwdisplay.h"
#include "qwrenderer.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_linux_dmabuf_v1.h>
}

QW_BEGIN_NAMESPACE

class QWLinuxDmabufV1Private : public QWObjectPrivate
{
public:
    QWLinuxDmabufV1Private(wlr_linux_dmabuf_v1 *handle, bool isOwner, QWLinuxDmabufV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWLinuxDmabufV1Private::on_destroy);
    }
    ~QWLinuxDmabufV1Private() {
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

    static QHash<void*, QWLinuxDmabufV1*> map;
    QW_DECLARE_PUBLIC(QWLinuxDmabufV1)
    QWSignalConnector sc;
};
QHash<void*, QWLinuxDmabufV1*> QWLinuxDmabufV1Private::map;

void QWLinuxDmabufV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWLinuxDmabufV1::QWLinuxDmabufV1(wlr_linux_dmabuf_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWLinuxDmabufV1Private(handle, isOwner, this))
{

}

QWLinuxDmabufV1 *QWLinuxDmabufV1::get(wlr_linux_dmabuf_v1 *handle)
{
    return QWLinuxDmabufV1Private::map.value(handle);
}

QWLinuxDmabufV1 *QWLinuxDmabufV1::from(wlr_linux_dmabuf_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWLinuxDmabufV1(handle, false);
}

#if WLR_VERSION_MINOR > 16

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

#else

QWLinuxDmabufV1 *QWLinuxDmabufV1::create(QWDisplay *display, QWRenderer *renderer)
{
    auto *handle = wlr_linux_dmabuf_v1_create(display->handle(), renderer->handle());
    if (!handle)
        return nullptr;
    return new QWLinuxDmabufV1(handle, true);
}

#endif // WLR_VERSION_MINOR > 16

QW_END_NAMESPACE
