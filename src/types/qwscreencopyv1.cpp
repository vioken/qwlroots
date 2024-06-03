// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwscreencopyv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_screencopy_v1.h>
}

QW_BEGIN_NAMESPACE

class QWScreenCopyManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWScreenCopyManagerV1Private(wlr_screencopy_manager_v1 *handle, bool isOwner, QWScreenCopyManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWScreenCopyManagerV1)
};

QWScreenCopyManagerV1::QWScreenCopyManagerV1(wlr_screencopy_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWScreenCopyManagerV1Private(handle, isOwner, this))
{

}

QWScreenCopyManagerV1 *QWScreenCopyManagerV1::get(wlr_screencopy_manager_v1 *handle)
{
    return static_cast<QWScreenCopyManagerV1*>(QWScreenCopyManagerV1Private::map.value(handle));
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
