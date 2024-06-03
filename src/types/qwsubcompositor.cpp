// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsubcompositor.h"
#include "qwcompositor.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_subcompositor.h>
}

QW_BEGIN_NAMESPACE

class QWSubcompositorPrivate : public QWWrapObjectPrivate
{
public:
    QWSubcompositorPrivate(wlr_subcompositor *handle, bool isOwner, QWSubcompositor *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWSubcompositor)
};

QWSubcompositor::QWSubcompositor(wlr_subcompositor *handle, bool isOwner)
    : QWWrapObject(*new QWSubcompositorPrivate(handle, isOwner, this))
{

}

QWSubcompositor *QWSubcompositor::get(wlr_subcompositor *handle)
{
    return static_cast<QWSubcompositor*>(QWSubcompositorPrivate::map.value(handle));
}

QWSubcompositor *QWSubcompositor::from(wlr_subcompositor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSubcompositor(handle, false);
}

QWSubcompositor *QWSubcompositor::create(QWDisplay *display)
{
    auto subcompositor = wlr_subcompositor_create(display->handle());
    if (!subcompositor)
        return nullptr;
    return new QWSubcompositor(subcompositor, true);
}

class QWSubsurfacePrivate : public QWWrapObjectPrivate
{
public:
    QWSubsurfacePrivate(wlr_subsurface *handle, bool isOwner, QWSubsurface *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWSubsurface)
};

QWSubsurface::QWSubsurface(wlr_subsurface *handle, bool isOwner)
    : QWWrapObject(*new QWSubsurfacePrivate(handle, isOwner, this))
{

}


QWSubsurface *QWSubsurface::get(wlr_subsurface *handle)
{
    return static_cast<QWSubsurface*>(QWSubsurfacePrivate::map.value(handle));
}

QWSubsurface *QWSubsurface::from(wlr_subsurface *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSubsurface(handle, false);
}

QWSubsurface *QWSubsurface::tryFrom(QWSurface *surface)
{
    auto handle = wlr_subsurface_try_from_wlr_surface(surface->handle());
    return handle ? from(handle) : nullptr;
}

QW_END_NAMESPACE
