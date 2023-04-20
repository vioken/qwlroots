// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <qwkeyboardinterface.h>
#include <QObject>
#include <type_traits>

struct wlr_keyboard;
struct wlr_keyboard_key_event;
struct wlr_input_device;
struct xkb_keymap;

QW_BEGIN_NAMESPACE

class QWKeyboardPrivate;
class QW_EXPORT QWKeyboard : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWKeyboard)
public:
    inline wlr_keyboard *handle() const {
        return QWObject::handle<wlr_keyboard>();
    }

    static QWKeyboard *get(wlr_keyboard *handle);
    static QWKeyboard *from(wlr_keyboard *handle);
    static QWKeyboard *fromInputDevice(wlr_input_device *input_device);

    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWKeyboardInterface, Interface>::value, QWKeyboard*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWKeyboardInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWKeyboard(i->handle(), true);
    }

    uint32_t getModifiers() const;
    void setKeymap(xkb_keymap *keymap);
    void setRepeatInfo(int32_t rate, int32_t delay);

Q_SIGNALS:
    void key(wlr_keyboard_key_event *event);
    void modifiers();
    void keymapChanged();
    void repeatInfoChanged();

private:
    ~QWKeyboard() override = default;
    QWKeyboard(wlr_keyboard *handle, bool isOwner);
};

QW_END_NAMESPACE
