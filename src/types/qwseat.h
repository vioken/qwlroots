// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

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
struct wlr_surface;

typedef int wlr_axis_orientation_t;
typedef int wlr_axis_source_t;
typedef int wlr_button_state_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWKeyboard;
class QWSeatPrivate;
class QW_EXPORT QWSeat : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSeat)
public:
    static QWSeat *create(QWDisplay *display, const char *name);

    inline wlr_seat *handle() const {
        return QWObject::handle<wlr_seat>();
    }

    static QWSeat *get(wlr_seat *handle);
    static QWSeat *from(wlr_seat *handle);

    void setKeyboard(QWKeyboard *keyboard);
    QWKeyboard *getKeyboard() const;
    void setCapabilities(uint32_t capabilities);
    void setSelection(wlr_data_source *source, uint32_t serial);
    void keyboardNotifyEnter(wlr_surface *surface, uint32_t keycodes[], size_t num_keycodes, wlr_keyboard_modifiers *modifiers);
    void keyboardNotifyKey(uint32_t time_msec, uint32_t key, uint32_t state);
    void keyboardNotifyModifiers(wlr_keyboard_modifiers *modifiers);
    void pointerNotifyAxis(uint32_t time_msec, wlr_axis_orientation_t orientation, double value, int32_t value_discrete, wlr_axis_source_t source);
    void pointerNotifyButton(uint32_t time_msec, uint32_t button, wlr_button_state_t state);
    void pointerNotifyEnter(wlr_surface *surface, double sx, double sy);
    void pointerNotifyFrame();
    void pointerNotifyMotion(uint32_t time_msec, double sx, double sy);
    void pointerClearFocus();

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
    ~QWSeat() = default;
};

QW_END_NAMESPACE
