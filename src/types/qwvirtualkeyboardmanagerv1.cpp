// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualkeyboardv1.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
}

QW_BEGIN_NAMESPACE

class QWVirtualKeyboardManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWVirtualKeyboardManagerV1Private(wlr_virtual_keyboard_manager_v1 *handle, bool isOwner, QWVirtualKeyboardManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_virtual_keyboard, this, &QWVirtualKeyboardManagerV1Private::on_new_virtual_keyboard);
    }

    void on_new_virtual_keyboard(wlr_virtual_keyboard_v1 *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWVirtualKeyboardManagerV1)
};
QHash<void*, QWWrapObject*> QWVirtualKeyboardManagerV1Private::map;

void QWVirtualKeyboardManagerV1Private::on_new_virtual_keyboard(wlr_virtual_keyboard_v1 *handle)
{
    Q_EMIT q_func()->newVirtualKeyboard(QWVirtualKeyboardV1::from(handle));
}

QWVirtualKeyboardManagerV1::QWVirtualKeyboardManagerV1(wlr_virtual_keyboard_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWVirtualKeyboardManagerV1Private(handle, isOwner, this))
{

}

QWVirtualKeyboardManagerV1 *QWVirtualKeyboardManagerV1::get(wlr_virtual_keyboard_manager_v1 *handle)
{
    return static_cast<QWVirtualKeyboardManagerV1*>(QWVirtualKeyboardManagerV1Private::map.value(handle));
}

QWVirtualKeyboardManagerV1 *QWVirtualKeyboardManagerV1::from(wlr_virtual_keyboard_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWVirtualKeyboardManagerV1(handle, false);
}

QWVirtualKeyboardManagerV1 *QWVirtualKeyboardManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_virtual_keyboard_manager_v1_create(display->handle());
    return handle ? new QWVirtualKeyboardManagerV1(handle, true) : nullptr;
}

QW_END_NAMESPACE
