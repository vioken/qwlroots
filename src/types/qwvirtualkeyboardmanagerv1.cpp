// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwvirtualkeyboardv1.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
}

QW_BEGIN_NAMESPACE

class QWVirtualKeyboardManagerV1Private : public QWObjectPrivate
{
public:
    QWVirtualKeyboardManagerV1Private(wlr_virtual_keyboard_manager_v1 *handle, bool isOwner, QWVirtualKeyboardManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWVirtualKeyboardManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_virtual_keyboard, this, &QWVirtualKeyboardManagerV1Private::on_new_virtual_keyboard);
    }
    ~QWVirtualKeyboardManagerV1Private() {
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
    void on_new_virtual_keyboard(wlr_virtual_keyboard_v1 *);

    static QHash<void*, QWVirtualKeyboardManagerV1*> map;
    QW_DECLARE_PUBLIC(QWVirtualKeyboardManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWVirtualKeyboardManagerV1*> QWVirtualKeyboardManagerV1Private::map;

void QWVirtualKeyboardManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWVirtualKeyboardManagerV1Private::on_new_virtual_keyboard(wlr_virtual_keyboard_v1 *handle)
{
    Q_EMIT q_func()->newVirtualKeyboard(QWVirtualKeyboardV1::from(handle));
}

QWVirtualKeyboardManagerV1::QWVirtualKeyboardManagerV1(wlr_virtual_keyboard_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWVirtualKeyboardManagerV1Private(handle, isOwner, this))
{

}

QWVirtualKeyboardManagerV1 *QWVirtualKeyboardManagerV1::get(wlr_virtual_keyboard_manager_v1 *handle)
{
    return QWVirtualKeyboardManagerV1Private::map.value(handle);
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
