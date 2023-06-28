// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_virtual_keyboard_manager_v1;
struct wlr_virtual_keyboard_v1;

QW_BEGIN_NAMESPACE

class QWInputDevice;
class QWVirtualKeyboardV1Private;
class QW_EXPORT QWVirtualKeyboardV1
{
public:
    QWVirtualKeyboardV1() = delete;
    ~QWVirtualKeyboardV1() = delete;

    static QWVirtualKeyboardV1 *from(wlr_virtual_keyboard_v1 *handle);
    static QWVirtualKeyboardV1 *fromInputDevice(QWInputDevice *inputDevice);
};

class QWDisplay;
class QWVirtualKeyboardManagerV1Private;
class QW_EXPORT QWVirtualKeyboardManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWVirtualKeyboardManagerV1)
public:
    inline wlr_virtual_keyboard_manager_v1 *handle() const {
        return QWObject::handle<wlr_virtual_keyboard_manager_v1>();
    }

    static QWVirtualKeyboardManagerV1 *get(wlr_virtual_keyboard_manager_v1 *handle);
    static QWVirtualKeyboardManagerV1 *from(wlr_virtual_keyboard_manager_v1 *handle);
    static QWVirtualKeyboardManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWVirtualKeyboardManagerV1 *self);
    void newVirtualKeyboard(QWVirtualKeyboardV1 *);

private:
    QWVirtualKeyboardManagerV1(wlr_virtual_keyboard_manager_v1 *handle, bool isOwner);
    ~QWVirtualKeyboardManagerV1() = default;
};

QW_END_NAMESPACE

