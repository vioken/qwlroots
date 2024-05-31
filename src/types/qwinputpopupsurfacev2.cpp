// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"

#include <QHash>
#include <QRect>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWInputPopupSurfaceV2Private : public QWWrapObjectPrivate
{
public:
    QWInputPopupSurfaceV2Private(wlr_input_popup_surface_v2 *handle, bool isOwner, QWInputPopupSurfaceV2 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWInputPopupSurfaceV2)
};
QHash<void*, QWWrapObject*> QWInputPopupSurfaceV2Private::map;

QWInputPopupSurfaceV2::QWInputPopupSurfaceV2(wlr_input_popup_surface_v2 *handle, bool isOwner)
    : QWWrapObject(*new QWInputPopupSurfaceV2Private(handle, isOwner, this))
{

}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::get(wlr_input_popup_surface_v2 *handle)
{
    return static_cast<QWInputPopupSurfaceV2*>(QWInputPopupSurfaceV2Private::map.value(handle));
}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::from(wlr_input_popup_surface_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputPopupSurfaceV2(handle, false);
}

QWInputPopupSurfaceV2 *QWInputPopupSurfaceV2::from(QWSurface *surface)
{
    auto *handle = wlr_input_popup_surface_v2_try_from_wlr_surface(surface->handle());
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
