// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcontenttypev1.h"
#include "qwcompositor.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_content_type_v1.h>
}

static_assert(std::is_same_v<wp_content_type_v1_type_t, std::underlying_type_t<wp_content_type_v1_type>>);

QW_BEGIN_NAMESPACE

class QWContentTypeManagerV1Private : public QWObjectPrivate
{
public:
    QWContentTypeManagerV1Private(wlr_content_type_manager_v1 *handle, bool isOwner, QWContentTypeManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWContentTypeManagerV1Private::on_destroy);
    }
    ~QWContentTypeManagerV1Private() {
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

    static QHash<void*, QWContentTypeManagerV1*> map;
    QW_DECLARE_PUBLIC(QWContentTypeManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWContentTypeManagerV1*> QWContentTypeManagerV1Private::map;

void QWContentTypeManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWContentTypeManagerV1::QWContentTypeManagerV1(wlr_content_type_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWContentTypeManagerV1Private(handle, isOwner, this))
{

}

QWContentTypeManagerV1 *QWContentTypeManagerV1::get(wlr_content_type_manager_v1 *handle)
{
    return QWContentTypeManagerV1Private::map.value(handle);
}

QWContentTypeManagerV1 *QWContentTypeManagerV1::from(wlr_content_type_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWContentTypeManagerV1(handle, false);
}

QWContentTypeManagerV1 *QWContentTypeManagerV1::create(QWDisplay *display, uint32_t version)
{
    auto *handle = wlr_content_type_manager_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWContentTypeManagerV1(handle, true);
}

wp_content_type_v1_type_t QWContentTypeManagerV1::getSurfaceContentType(QWSurface *surface)
{
    return static_cast<wp_content_type_v1_type_t>(wlr_surface_get_content_type_v1(handle(), surface->handle()));
}

QW_END_NAMESPACE
