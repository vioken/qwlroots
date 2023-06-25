// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_keyboard_shortcuts_inhibitor_v1;
struct wlr_keyboard_shortcuts_inhibit_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWKeyboardShortcutsInhibitorV1Private;
class QW_EXPORT QWKeyboardShortcutsInhibitorV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWKeyboardShortcutsInhibitorV1)
public:
    inline wlr_keyboard_shortcuts_inhibitor_v1 *handle() const {
        return QWObject::handle<wlr_keyboard_shortcuts_inhibitor_v1>();
    }

    static QWKeyboardShortcutsInhibitorV1 *get(wlr_keyboard_shortcuts_inhibitor_v1 *handle);
    static QWKeyboardShortcutsInhibitorV1 *from(wlr_keyboard_shortcuts_inhibitor_v1 *handle);

    void activate();
    void deactivate();

Q_SIGNALS:
    void beforeDestroy(QWKeyboardShortcutsInhibitorV1 *self);

private:
    QWKeyboardShortcutsInhibitorV1(wlr_keyboard_shortcuts_inhibitor_v1 *handle, bool isOwner);
    ~QWKeyboardShortcutsInhibitorV1() = default;
};

class QWKeyboardShortcutsInhibitManagerV1Private;
class QW_EXPORT QWKeyboardShortcutsInhibitManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWKeyboardShortcutsInhibitManagerV1)
public:
    inline wlr_keyboard_shortcuts_inhibit_manager_v1 *handle() const {
        return QWObject::handle<wlr_keyboard_shortcuts_inhibit_manager_v1>();
    }

    static QWKeyboardShortcutsInhibitManagerV1 *get(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle);
    static QWKeyboardShortcutsInhibitManagerV1 *from(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle);
    static QWKeyboardShortcutsInhibitManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWKeyboardShortcutsInhibitManagerV1 *self);
    void newInhibitor(QWKeyboardShortcutsInhibitorV1 *inhibitor);

private:
    QWKeyboardShortcutsInhibitManagerV1(wlr_keyboard_shortcuts_inhibit_manager_v1 *handle, bool isOwner);
    ~QWKeyboardShortcutsInhibitManagerV1() = default;
};

QW_END_NAMESPACE

