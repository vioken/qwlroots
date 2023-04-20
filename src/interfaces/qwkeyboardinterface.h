// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_keyboard;
struct wlr_keyboard_impl;

QW_BEGIN_NAMESPACE

class QW_EXPORT QWKeyboardInterface : public QWInterface
{
    friend class QWKeyboard;
public:
    virtual ~QWKeyboardInterface();

    virtual const char* name() = 0;

    virtual void ledUpdate(uint32_t leds) const;

    inline wlr_keyboard *handle() const {
        return QWInterface::handle<wlr_keyboard>();
    }
    inline wlr_keyboard_impl *impl() const {
        return QWInterface::handle<wlr_keyboard_impl>();
    }
    static QWKeyboardInterface *get(wlr_keyboard *handle);

protected:
    template<class T>
    inline void init() {
        init(getFuncMagicKey<T>(&T::ledUpdate));
    }

    virtual void init(FuncMagicKey funMagicKey);
};

QW_END_NAMESPACE
