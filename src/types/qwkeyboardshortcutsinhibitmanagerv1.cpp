// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardshortcutsinhibitv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_shortcuts_inhibit_v1.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardShortcutsInhibitManagerV1Private : public QWObjectPrivate
{
public:
    QWKeyboardShortcutsInhibitManagerV1Private(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle, bool isOwner, QWKeyboardShortcutsInhibitManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWKeyboardShortcutsInhibitManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_inhibitor, this, &QWKeyboardShortcutsInhibitManagerV1Private::on_new_inhibitor);
    }
    ~QWKeyboardShortcutsInhibitManagerV1Private() {
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
    void on_new_inhibitor(void *);

    static QHash<void*, QWKeyboardShortcutsInhibitManagerV1*> map;
    QW_DECLARE_PUBLIC(QWKeyboardShortcutsInhibitManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWKeyboardShortcutsInhibitManagerV1*> QWKeyboardShortcutsInhibitManagerV1Private::map;

void QWKeyboardShortcutsInhibitManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWKeyboardShortcutsInhibitManagerV1Private::on_new_inhibitor(void *data)
{
    auto *inhibitor = QWKeyboardShortcutsInhibitorV1::from(reinterpret_cast<wlr_keyboard_shortcuts_inhibitor_v1*>(data));
    Q_EMIT q_func()->newInhibitor(inhibitor);
}

QWKeyboardShortcutsInhibitManagerV1::QWKeyboardShortcutsInhibitManagerV1(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWKeyboardShortcutsInhibitManagerV1Private(handle, isOwner, this))
{

}

QWKeyboardShortcutsInhibitManagerV1 *QWKeyboardShortcutsInhibitManagerV1::get(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle)
{
    return QWKeyboardShortcutsInhibitManagerV1Private::map.value(handle);
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
