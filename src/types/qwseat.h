// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <cstdint>
#include <qwglobal.h>
#include <QObject>

struct wlr_seat;
struct wlr_seat_pointer_request_set_cursor_event;
struct wlr_seat_request_set_selection_event;
struct wlr_seat_request_set_primary_selection_event;
struct wlr_seat_request_start_drag_event;
struct wlr_drag;
struct wlr_data_source;
struct wlr_keyboard_modifiers;
struct wlr_seat_keyboard_grab;
struct wlr_seat_pointer_grab;
struct wlr_seat_touch_grab;
struct wlr_touch_point;

typedef uint32_t wlr_axis_orientation_t;
typedef uint32_t wlr_axis_source_t;
typedef uint32_t wlr_button_state_t;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWDisplay;
class QWKeyboard;
class QWSeatPrivate;
class QW_EXPORT QWSeat : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSeat)
public:
    ~QWSeat() = default;

    inline wlr_seat *handle() const {
        return QWObject::handle<wlr_seat>();
    }

    static QWSeat *get(wlr_seat *handle);
    static QWSeat *from(wlr_seat *handle);
    static QWSeat *create(QWDisplay *display, const char *name);

    void setKeyboard(QWKeyboard *keyboard);
    QWKeyboard *getKeyboard() const;
    void setCapabilities(uint32_t capabilities);
    void setSelection(wlr_data_source *source, uint32_t serial);
    void keyboardClearFocus();
    void keyboardEndGrab();
#if WLR_VERSION_MINOR > 16
    void keyboardEnter(QWSurface *surface, const uint32_t keycodes[], size_t numKeycodes, const wlr_keyboard_modifiers *modifiers);
#else
    void keyboardEnter(QWSurface *surface, uint32_t keycodes[], size_t numKeycodes, wlr_keyboard_modifiers *modifiers);
#endif
    bool keyboardHasGrab() const;
    void keyboardNotifyClearFocus();
    void keyboardNotifyEnter(QWSurface *surface, uint32_t keycodes[], size_t numKeycodes, wlr_keyboard_modifiers *modifiers);
    void keyboardNotifyKey(uint32_t time_msec, uint32_t key, uint32_t state);
    void keyboardNotifyModifiers(wlr_keyboard_modifiers *modifiers);
    void keyboardSendKey(uint32_t timeMsec, uint32_t key, uint32_t state);
    void keyboardSendModifiers(wlr_keyboard_modifiers *modifiers);
    void keyboardStartGrab(wlr_seat_keyboard_grab *grab);
    void pointerEndGrab();
    void pointerEnter(QWSurface *surface, double sx, double sy);
    bool pointerHasGrab() const;
    void pointerNotifyAxis(uint32_t time_msec, wlr_axis_orientation_t orientation, double value, int32_t value_discrete, wlr_axis_source_t source);
    void pointerNotifyButton(uint32_t time_msec, uint32_t button, wlr_button_state_t state);
    void pointerNotifyClearFocus();
    void pointerNotifyEnter(QWSurface *surface, double sx, double sy);
    void pointerNotifyFrame();
    void pointerNotifyMotion(uint32_t time_msec, double sx, double sy);
    void pointerSendAxis(uint32_t timeMsec, wlr_axis_orientation_t orientation, double value, int32_t valueDiscrete, wlr_axis_source_t source);
    uint32_t pointerSendButton(uint32_t timeMsec, uint32_t button, wlr_button_state_t state);
    void pointerSendFrame();
    void pointerSendMotion(uint32_t timeMsec, double sx, double sy);
    void pointerStartGrab(wlr_seat_pointer_grab *grab);
    bool pointerSurfaceHasFocus(QWSurface *surface) const;
    void pointerWarp(double sx, double sy);
    void pointerClearFocus();
    void setName(const char *name);
    void touchEndGrab();
    wlr_touch_point *touchGetPoint(int32_t touchId) const;
    bool touchHasGrab() const;
    void touchNotifyCancel(QWSurface *surface);
    uint32_t touchNotifyDown(QWSurface *surface, uint32_t timeMsec, int32_t touch_id, double sx, double sy);
    void touchNotifyFrame();
    void touchNotifyMotion(uint32_t timeMsec, int32_t touchId, double sx, double sy);
    void touchNotifyUp(uint32_t timeMsec, int32_t touchId);
    int touchNumPoints();
    void touchPointClearFocus(uint32_t timeMsec, int32_t touchId);
    void touchPointFocus(QWSurface *surface, uint32_t timeMsec, int32_t touchId, double sx, double sy);
    void touchSendCancel(QWSurface *surface);
    uint32_t touchSendDown(QWSurface *surface, uint32_t timeMsec, int32_t touchId, double sx, double sy);
    void touchSendFrame();
    void touchSendMotion(uint32_t timeMsec, int32_t touchId, double sx, double sy);
    void touchSendUp(uint32_t timeMsec, int32_t touchId);
    void touchStartGrab(wlr_seat_touch_grab *grab);
    bool validaiteGrabSerial(uint32_t serial);
    bool validatePointerGrabSerial(QWSurface *origin, uint32_t serial);
    bool validateTouchGrabSerial(QWSurface *origin, uint32_t serial, wlr_touch_point **pointPtr);
    bool surfaceAcceptsTouch(QWSurface *surface);

Q_SIGNALS:
    void beforeDestroy(QWSeat *self);
    void pointerGrabBegin();
    void pointerGrabEnd();
    void keyboardGrabBegin();
    void keyboardGrabEnd();
    void touchGrabBegin();
    void touchGrabEnd();
    void requestSetCursor(wlr_seat_pointer_request_set_cursor_event *event);
    void requestSetSelection(wlr_seat_request_set_selection_event *event);
    void selectionChanged();
    void requestSetPrimarySelection(wlr_seat_request_set_primary_selection_event *event);
    void primarySelectionChanged();
    void requestStartDrag(wlr_seat_request_start_drag_event *event);
    void startDrag(wlr_drag *drag);

private:
    QWSeat(wlr_seat *handle, bool isOwner);
};

QW_END_NAMESPACE
