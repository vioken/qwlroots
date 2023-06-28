// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_input_method_keyboard_grab_v2;
struct wlr_keyboard_modifiers;
struct wlr_input_method_v2;
struct wlr_input_popup_surface_v2;

QW_BEGIN_NAMESPACE

class QWKeyboard;
class QWSurface;
class QWInputMethodKeyboardGrabV2Private;
class QW_EXPORT QWInputMethodKeyboardGrabV2 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWInputMethodKeyboardGrabV2)
public:
    ~QWInputMethodKeyboardGrabV2() = default;

    inline wlr_input_method_keyboard_grab_v2 *handle() const {
        return QWObject::handle<wlr_input_method_keyboard_grab_v2>();
    }

    static QWInputMethodKeyboardGrabV2 *get(wlr_input_method_keyboard_grab_v2 *handle);
    static QWInputMethodKeyboardGrabV2 *from(wlr_input_method_keyboard_grab_v2 *handle);

    void sendKey(uint32_t time, uint32_t key, uint32_t state);
    void sendModifiers(wlr_keyboard_modifiers *modifiers);
    void setKeyboard(QWKeyboard *keyboard);

Q_SIGNALS:
    void beforeDestroy(QWInputMethodKeyboardGrabV2 *self);

private:
    QWInputMethodKeyboardGrabV2(wlr_input_method_keyboard_grab_v2 *handle, bool isOwner);
};

class QWInputPopupSurfaceV2Private;
class QW_EXPORT QWInputPopupSurfaceV2 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWInputPopupSurfaceV2)
public:
    inline wlr_input_popup_surface_v2 *handle() const {
        return QWObject::handle<wlr_input_popup_surface_v2>();
    }

    static QWInputPopupSurfaceV2 *get(wlr_input_popup_surface_v2 *handle);
    static QWInputPopupSurfaceV2 *from(wlr_input_popup_surface_v2 *handle);
    static QWInputPopupSurfaceV2 *from(QWSurface *handle);

    QWSurface *surface() const;

    void send_text_input_rectangle(const QRect &sbox);

Q_SIGNALS:
    void beforeDestroy(QWInputPopupSurfaceV2 *self);

private:
    QWInputPopupSurfaceV2(wlr_input_popup_surface_v2 *handle, bool isOwner);
    ~QWInputPopupSurfaceV2() = default;
};

class QWInputMethodV2Private;
class QW_EXPORT QWInputMethodV2 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWInputMethodV2)
public:
    inline wlr_input_method_v2 *handle() const {
        return QWObject::handle<wlr_input_method_v2>();
    }

    static QWInputMethodV2 *get(wlr_input_method_v2 *handle);
    static QWInputMethodV2 *from(wlr_input_method_v2 *handle);

    void sendActivate();
    void sendContentType(uint32_t hint, uint32_t purpose);
    void sendDeactivate();
    void sendDone();
    void sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor);
    void sendTextChangeCause(uint32_t cause);
    void sendUnavailable();

Q_SIGNALS:
    void beforeDestroy(QWInputMethodV2 *self);
    void commit(QWInputMethodV2 *inputMethod);
    void newPopupSurface(QWInputPopupSurfaceV2 *surface);
    void grabKeybord(QWInputMethodKeyboardGrabV2 *keyboardGrab);

private:
    QWInputMethodV2(wlr_input_method_v2 *handle, bool isOwner);
    ~QWInputMethodV2() = default;
};

QW_END_NAMESPACE

