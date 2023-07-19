// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"

#include <QHash>
#include <QRect>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWInputPopupSurfaceV2Private : public QWObjectPrivate
{
public:
    QWInputPopupSurfaceV2Private(wlr_input_popup_surface_v2 *handle, bool isOwner, QWInputPopupSurfaceV2 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWInputPopupSurfaceV2Private::on_destroy);
#if WLR_VERSION_MINOR <= 16
        sc.connect(&handle->events.map, this, &QWInputPopupSurfaceV2Private::on_map);
        sc.connect(&handle->events.unmap, this, &QWInputPopupSurfaceV2Private::on_unmap);
#endif
    }
    ~QWInputPopupSurfaceV2Private() {
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

#if WLR_VERSION_MINOR <= 16
    void on_map(void *);
    void on_unmap(void *);
#endif

    static QHash<void*, QWInputPopupSurfaceV2*> map;
    QW_DECLARE_PUBLIC(QWInputPopupSurfaceV2)
    QWSignalConnector sc;
};
QHash<void*, QWInputPopupSurfaceV2*> QWInputPopupSurfaceV2Private::map;

void QWInputPopupSurfaceV2Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

#if WLR_VERSION_MINOR <= 16
void QWInputPopupSurfaceV2Private::on_map(void *)
{
    Q_EMIT q_func()->surface()->map();
}

void QWInputPopupSurfaceV2Private::on_unmap(void *)
{
    Q_EMIT q_func()->surface()->unmap();
}
#endif

QWInputPopupSurfaceV2::QWInputPopupSurfaceV2(wlr_input_popup_surface_v2 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWInputPopupSurfaceV2Private(handle, isOwner, this))
{

}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::get(wlr_input_popup_surface_v2 *handle)
{
    return QWInputPopupSurfaceV2Private::map.value(handle);
}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::from(wlr_input_popup_surface_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputPopupSurfaceV2(handle, false);
}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::from(QWSurface *surface)
{
#if WLR_VERSION_MINOR > 16
    auto *handle = wlr_input_popup_surface_v2_try_from_wlr_surface(surface->handle());
#else
    if (!wlr_surface_is_input_popup_surface_v2(surface->handle()))
        return nullptr;
    auto *handle = wlr_input_popup_surface_v2_from_wlr_surface(surface->handle());
#endif
    if (!handle)
        return nullptr;
    return from(handle);
}

void QWInputPopupSurfaceV2::send_text_input_rectangle(const QRect &sbox)
{
    wlr_box b {
        .x = sbox.x(),
        .y = sbox.y(),
        .width = sbox.width(),
        .height = sbox.height()
    };
    wlr_input_popup_surface_v2_send_text_input_rectangle(handle(), &b);
}

QWSurface *QWInputPopupSurfaceV2::surface() const
{
    return QWSurface::from(handle()->surface);
}

QW_END_NAMESPACE
