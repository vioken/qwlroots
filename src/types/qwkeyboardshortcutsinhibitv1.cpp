// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardshortcutsinhibitv1.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_shortcuts_inhibit_v1.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardShortcutsInhibitorV1Private : public QWObjectPrivate
{
public:
    QWKeyboardShortcutsInhibitorV1Private(wlr_keyboard_shortcuts_inhibitor_v1 *handle, bool isOwner, QWKeyboardShortcutsInhibitorV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWKeyboardShortcutsInhibitorV1Private::on_destroy);
    }
    ~QWKeyboardShortcutsInhibitorV1Private() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWKeyboardShortcutsInhibitorV1*> map;
    QW_DECLARE_PUBLIC(QWKeyboardShortcutsInhibitorV1)
    QWSignalConnector sc;
};
QHash<void*, QWKeyboardShortcutsInhibitorV1*> QWKeyboardShortcutsInhibitorV1Private::map;

void QWKeyboardShortcutsInhibitorV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWKeyboardShortcutsInhibitorV1::QWKeyboardShortcutsInhibitorV1(wlr_keyboard_shortcuts_inhibitor_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWKeyboardShortcutsInhibitorV1Private(handle, isOwner, this))
{

}

QWKeyboardShortcutsInhibitorV1 *QWKeyboardShortcutsInhibitorV1::get(wlr_keyboard_shortcuts_inhibitor_v1 *handle)
{
    return QWKeyboardShortcutsInhibitorV1Private::map.value(handle);
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
