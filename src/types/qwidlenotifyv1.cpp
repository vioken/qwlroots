// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwidlenotifyv1.h"
#include "qwdisplay.h"
#include "qwseat.h"

extern "C" {
#include <wlr/types/wlr_idle_notify_v1.h>
}

QW_BEGIN_NAMESPACE

wlr_idle_notifier_v1 *QWIdleNotifierV1::handle() const
{
    return reinterpret_cast<wlr_idle_notifier_v1*>(const_cast<QWIdleNotifierV1*>(this));
}

QWIdleNotifierV1 *QWIdleNotifierV1::from(wlr_idle_notifier_v1 *handle)
{
    return reinterpret_cast<QWIdleNotifierV1*>(handle);
}

QWIdleNotifierV1 *QWIdleNotifierV1::create(QWDisplay *display)
{
    auto *handle = wlr_idle_notifier_v1_create(display->handle());
    return from(handle);
}

void QWIdleNotifierV1::notifyActivity(QWSeat *seat)
{
    wlr_idle_notifier_v1_notify_activity(handle(), seat->handle());
}

void QWIdleNotifierV1::setInhibited(bool inhibited)
{
    wlr_idle_notifier_v1_set_inhibited(handle(), inhibited);
}

QW_END_NAMESPACE
