// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_screencopy_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWScreenCopyManagerV1Private;
class QW_EXPORT QWScreenCopyManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWScreenCopyManagerV1)
public:
    inline wlr_screencopy_manager_v1 *handle() const {
        return QWObject::handle<wlr_screencopy_manager_v1>();
    }

    static QWScreenCopyManagerV1 *get(wlr_screencopy_manager_v1 *handle);
    static QWScreenCopyManagerV1 *from(wlr_screencopy_manager_v1 *handle);
    static QWScreenCopyManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWScreenCopyManagerV1 *self);

private:
    QWScreenCopyManagerV1(wlr_screencopy_manager_v1 *handle, bool isOwner);
    ~QWScreenCopyManagerV1() = default;
};

QW_END_NAMESPACE

