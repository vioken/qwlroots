// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_virtual_pointer_manager_v1;
struct wlr_virtual_pointer_v1_new_pointer_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWVirtualPointerManagerV1Private;
class QW_EXPORT QWVirtualPointerManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWVirtualPointerManagerV1)
public:
    inline wlr_virtual_pointer_manager_v1 *handle() const {
        return QWObject::handle<wlr_virtual_pointer_manager_v1>();
    }

    static QWVirtualPointerManagerV1 *get(wlr_virtual_pointer_manager_v1 *handle);
    static QWVirtualPointerManagerV1 *from(wlr_virtual_pointer_manager_v1 *handle);
    static QWVirtualPointerManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWVirtualPointerManagerV1 *self);
    void newVirtualPointer(wlr_virtual_pointer_v1_new_pointer_event *);

private:
    QWVirtualPointerManagerV1(wlr_virtual_pointer_manager_v1 *handle, bool isOwner);
    ~QWVirtualPointerManagerV1() = default;
};

QW_END_NAMESPACE
