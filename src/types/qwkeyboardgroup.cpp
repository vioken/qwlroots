// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardgroup.h"
#include "private/qwglobal_p.h"
#include "qwkeyboard.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_group.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardGroupPrivate : public QWWrapObjectPrivate
{
public:
    QWKeyboardGroupPrivate(wlr_keyboard_group *handle, bool isOwner, QWKeyboardGroup *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, nullptr,
                              toDestroyFunction(wlr_keyboard_group_destroy))
    {
        sc.connect(&handle->events.enter, this, &QWKeyboardGroupPrivate::on_enter);
        sc.connect(&handle->events.leave, this, &QWKeyboardGroupPrivate::on_leave);
    }

    void on_enter(void *);
    void on_leave(void *);

    QW_DECLARE_PUBLIC(QWKeyboardGroup)
};

void QWKeyboardGroupPrivate::on_enter(void *data)
{
    Q_EMIT q_func()->enter(reinterpret_cast<wl_array*>(data));
}

void QWKeyboardGroupPrivate::on_leave(void *data)
{
    Q_EMIT q_func()->leave(reinterpret_cast<wl_array*>(data));
}

QWKeyboardGroup::QWKeyboardGroup(wlr_keyboard_group *handle, bool isOwner, QObject *parent)
    : QWWrapObject(*new QWKeyboardGroupPrivate(handle, isOwner, this), parent)
{

}

QWKeyboardGroup::QWKeyboardGroup(QObject *parent)
    :QWKeyboardGroup(wlr_keyboard_group_create(), true, parent)
{

}

QWKeyboardGroup *QWKeyboardGroup::get(wlr_keyboard_group *handle)
{
    return static_cast<QWKeyboardGroup*>(QWKeyboardGroupPrivate::map.value(handle));
}

QWKeyboardGroup *QWKeyboardGroup::from(wlr_keyboard_group *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWKeyboardGroup(handle, false);
}

QWKeyboardGroup *QWKeyboardGroup::from(QWKeyboard  *keyboard)
{
    auto *handle = wlr_keyboard_group_from_wlr_keyboard(keyboard->handle());
    if (!handle)
        return nullptr;
    return from(handle);
}

void QWKeyboardGroup::addKeyboard(QWKeyboard *keyboard)
{
    wlr_keyboard_group_add_keyboard(handle(), keyboard->handle());
}

void QWKeyboardGroup::removeKeyboard(QWKeyboard *keyboard)
{
    wlr_keyboard_group_remove_keyboard(handle(), keyboard->handle());
}

QW_END_NAMESPACE
