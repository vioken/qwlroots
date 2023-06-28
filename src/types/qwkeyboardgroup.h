// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_keyboard_group;
struct wl_array;

QW_BEGIN_NAMESPACE

class QWKeyboard;
class QWKeyboardGroupPrivate;
class QW_EXPORT QWKeyboardGroup : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWKeyboardGroup)
public:
    explicit QWKeyboardGroup(QObject *parent = nullptr);
    ~QWKeyboardGroup() = default;

    inline wlr_keyboard_group *handle() const {
        return QWObject::handle<wlr_keyboard_group>();
    }

    static QWKeyboardGroup *get(wlr_keyboard_group *handle);
    static QWKeyboardGroup *from(wlr_keyboard_group *handle);
    static QWKeyboardGroup *from(QWKeyboard  *keyboard);
    void addKeyboard(QWKeyboard *keyboard);
    void removeKeyboard(QWKeyboard *keyboard);

Q_SIGNALS:
    void beforeDestroy(QWKeyboardGroup *self);
    void enter(wl_array *keycodes);
    void leave(wl_array *keycodes);

private:
    QWKeyboardGroup(wlr_keyboard_group *handle, bool isOwner, QObject *parent = nullptr);
};

QW_END_NAMESPACE
