// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwkeyboardgroup.h"
#include "util/qwsignalconnector.h"
#include "qwkeyboard.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_keyboard_group.h>
}

QW_BEGIN_NAMESPACE

class QWKeyboardGroupPrivate : public QWObjectPrivate
{
public:
    QWKeyboardGroupPrivate(wlr_keyboard_group *handle, bool isOwner, QWKeyboardGroup *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.enter, this, &QWKeyboardGroupPrivate::on_enter);
        sc.connect(&handle->events.leave, this, &QWKeyboardGroupPrivate::on_leave);
    }
    ~QWKeyboardGroupPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_keyboard_group_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_enter(void *);
    void on_leave(void *);

    static QHash<void*, QWKeyboardGroup*> map;
    QW_DECLARE_PUBLIC(QWKeyboardGroup)
    QWSignalConnector sc;
};
QHash<void*, QWKeyboardGroup*> QWKeyboardGroupPrivate::map;

void QWKeyboardGroupPrivate::on_enter(void *data)
{
    Q_EMIT q_func()->enter(reinterpret_cast<wl_array*>(data));
}

void QWKeyboardGroupPrivate::on_leave(void *data)
{
    Q_EMIT q_func()->leave(reinterpret_cast<wl_array*>(data));
}

QWKeyboardGroup::QWKeyboardGroup(wlr_keyboard_group *handle, bool isOwner, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWKeyboardGroupPrivate(handle, isOwner, this))
{

}

QWKeyboardGroup::QWKeyboardGroup(QObject *parent)
    :QWKeyboardGroup(wlr_keyboard_group_create(), true, parent)
{

}

QWKeyboardGroup *QWKeyboardGroup::get(wlr_keyboard_group *handle)
{
    return QWKeyboardGroupPrivate::map.value(handle);
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
