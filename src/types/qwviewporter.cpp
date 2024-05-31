// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwviewporter.h"
#include "private/qwglobal_p.h"
#include "qwdisplay.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_viewporter.h>
}

QW_BEGIN_NAMESPACE

class QWViewPorterPrivate : public QWWrapObjectPrivate
{
public:
    QWViewPorterPrivate(wlr_viewporter *handle, bool isOwner, QWViewPorter *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWViewPorter)
};
QHash<void*, QWWrapObject*> QWViewPorterPrivate::map;

QWViewPorter::QWViewPorter(wlr_viewporter *handle, bool isOwner)
    : QWWrapObject(*new QWViewPorterPrivate(handle, isOwner, this))
{

}

QWViewPorter *QWViewPorter::get(wlr_viewporter *handle)
{
    return static_cast<QWViewPorter*>(QWViewPorterPrivate::map.value(handle));
}

QWViewPorter *QWViewPorter::from(wlr_viewporter *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWViewPorter(handle, false);
}

QWViewPorter *QWViewPorter::create(QWDisplay *display)
{
    auto *handle = wlr_viewporter_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWViewPorter(handle, true);
}

QW_END_NAMESPACE
