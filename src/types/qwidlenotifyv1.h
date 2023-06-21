// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_idle_notifier_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSeat;
class QW_EXPORT QWIdleNotifierV1
{
public:
    QWIdleNotifierV1() = delete;
    ~QWIdleNotifierV1() = delete;

    wlr_idle_notifier_v1 *handle() const;

    static QWIdleNotifierV1 *from(wlr_idle_notifier_v1 *handle);
    static QWIdleNotifierV1 *create(QWDisplay *display);

    void notifyActivity(QWSeat *seat);
    void setInhibited(bool inhibited);
};

QW_END_NAMESPACE
