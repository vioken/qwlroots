// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardshortcutsinhibitv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_shortcuts_inhibit_v1.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardShortcutsInhibitManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWKeyboardShortcutsInhibitManagerV1Private(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle, bool isOwner, QWKeyboardShortcutsInhibitManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.new_inhibitor, this, &QWKeyboardShortcutsInhibitManagerV1Private::on_new_inhibitor);
    }

    void on_new_inhibitor(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWKeyboardShortcutsInhibitManagerV1)
};
QHash<void*, QWWrapObject*> QWKeyboardShortcutsInhibitManagerV1Private::map;

void QWKeyboardShortcutsInhibitManagerV1Private::on_new_inhibitor(void *data)
{
    auto *inhibitor = QWKeyboardShortcutsInhibitorV1::from(reinterpret_cast<wlr_keyboard_shortcuts_inhibitor_v1*>(data));
    Q_EMIT q_func()->newInhibitor(inhibitor);
}

QWKeyboardShortcutsInhibitManagerV1::QWKeyboardShortcutsInhibitManagerV1(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWKeyboardShortcutsInhibitManagerV1Private(handle, isOwner, this))
{

}

QWKeyboardShortcutsInhibitManagerV1 *QWKeyboardShortcutsInhibitManagerV1::get(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle)
{
    return static_cast<QWKeyboardShortcutsInhibitManagerV1*>(QWKeyboardShortcutsInhibitManagerV1Private::map.value(handle));
}

QWKeyboardShortcutsInhibitManagerV1 *QWKeyboardShortcutsInhibitManagerV1::from(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWKeyboardShortcutsInhibitManagerV1(handle, false);
}

QWKeyboardShortcutsInhibitManagerV1 *QWKeyboardShortcutsInhibitManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_keyboard_shortcuts_inhibit_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWKeyboardShortcutsInhibitManagerV1(handle, true);
}

QW_END_NAMESPACE
