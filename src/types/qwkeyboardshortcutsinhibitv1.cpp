// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardshortcutsinhibitv1.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_shortcuts_inhibit_v1.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardShortcutsInhibitorV1Private : public QWWrapObjectPrivate
{
public:
    QWKeyboardShortcutsInhibitorV1Private(wlr_keyboard_shortcuts_inhibitor_v1 *handle, bool isOwner, QWKeyboardShortcutsInhibitorV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {

    }

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWKeyboardShortcutsInhibitorV1)
};
QHash<void*, QWWrapObject*> QWKeyboardShortcutsInhibitorV1Private::map;

QWKeyboardShortcutsInhibitorV1::QWKeyboardShortcutsInhibitorV1(wlr_keyboard_shortcuts_inhibitor_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWKeyboardShortcutsInhibitorV1Private(handle, isOwner, this))
{

}

QWKeyboardShortcutsInhibitorV1 *QWKeyboardShortcutsInhibitorV1::get(wlr_keyboard_shortcuts_inhibitor_v1 *handle)
{
    return static_cast<QWKeyboardShortcutsInhibitorV1*>(QWKeyboardShortcutsInhibitorV1Private::map.value(handle));
}

QWKeyboardShortcutsInhibitorV1 *QWKeyboardShortcutsInhibitorV1::from(wlr_keyboard_shortcuts_inhibitor_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWKeyboardShortcutsInhibitorV1(handle, false);
}

void QWKeyboardShortcutsInhibitorV1::activate()
{
    wlr_keyboard_shortcuts_inhibitor_v1_activate(handle());
}

void QWKeyboardShortcutsInhibitorV1::deactivate()
{
    wlr_keyboard_shortcuts_inhibitor_v1_deactivate(handle());
}

QW_END_NAMESPACE
