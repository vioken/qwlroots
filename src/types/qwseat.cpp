// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwseat.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_pointer.h>
}

QW_BEGIN_NAMESPACE

class QWSeatPrivate : public QWObjectPrivate
{
public:
    QWSeatPrivate(wlr_seat *handle, bool isOwner, QWSeat *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.pointer_grab_begin, this, &QWSeatPrivate::on_pointer_grab_begin);
        sc.connect(&handle->events.pointer_grab_end, this, &QWSeatPrivate::on_pointer_grab_end);
        sc.connect(&handle->events.keyboard_grab_begin, this, &QWSeatPrivate::on_keyboard_grab_begin);
        sc.connect(&handle->events.keyboard_grab_end, this, &QWSeatPrivate::on_keyboard_grab_end);
        sc.connect(&handle->events.touch_grab_begin, this, &QWSeatPrivate::on_touch_grab_begin);
        sc.connect(&handle->events.touch_grab_end, this, &QWSeatPrivate::on_touch_grab_end);
        sc.connect(&handle->events.request_set_cursor, this, &QWSeatPrivate::on_request_set_cursor);
        sc.connect(&handle->events.request_set_selection, this, &QWSeatPrivate::on_request_set_selection);
        sc.connect(&handle->events.set_selection, this, &QWSeatPrivate::on_set_selection);
        sc.connect(&handle->events.request_set_primary_selection, this, &QWSeatPrivate::on_request_set_primary_selection);
        sc.connect(&handle->events.set_primary_selection, this, &QWSeatPrivate::on_set_primary_selection);
        sc.connect(&handle->events.request_start_drag, this, &QWSeatPrivate::on_request_start_drag);
        sc.connect(&handle->events.start_drag, this, &QWSeatPrivate::on_start_drag);
        sc.connect(&handle->events.destroy, this, &QWSeatPrivate::on_destroy);
    }
    ~QWSeatPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_seat_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_pointer_grab_begin(void *);
    void on_pointer_grab_end(void *);
    void on_keyboard_grab_begin(void *);
    void on_keyboard_grab_end(void *);
    void on_touch_grab_begin(void *);
    void on_touch_grab_end(void *);
    void on_request_set_cursor(void *);
    void on_request_set_selection(void *);
    void on_set_selection(void *);
    void on_destroy(void *);
    void on_request_set_primary_selection(void *);
    void on_set_primary_selection(void *);
    void on_request_start_drag(void *);
    void on_start_drag(void *);

    static QHash<void*, QWSeat*> map;
    QW_DECLARE_PUBLIC(QWSeat)
    QWSignalConnector sc;
};
QHash<void*, QWSeat*> QWSeatPrivate::map;

void QWSeatPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWSeatPrivate::on_pointer_grab_begin(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->pointerGrabBegin();
}

void QWSeatPrivate::on_pointer_grab_end(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->pointerGrabEnd();
}

void QWSeatPrivate::on_touch_grab_begin(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->touchGrabBegin();
}

void QWSeatPrivate::on_touch_grab_end(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->touchGrabEnd();
}

void QWSeatPrivate::on_keyboard_grab_begin(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->keyboardGrabBegin();
}

void QWSeatPrivate::on_keyboard_grab_end(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->keyboardGrabEnd();
}

void QWSeatPrivate::on_request_set_cursor(void *data)
{
    Q_EMIT q_func()->requestSetCursor(reinterpret_cast<wlr_seat_pointer_request_set_cursor_event*>(data));
}

void QWSeatPrivate::on_request_set_selection(void *data)
{
    Q_EMIT q_func()->requestSetSelection(reinterpret_cast<wlr_seat_request_set_selection_event*>(data));
}

void QWSeatPrivate::on_set_selection(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->selectionChanged();
}

void QWSeatPrivate::on_request_set_primary_selection(void *data)
{
    Q_EMIT q_func()->requestSetPrimarySelection(reinterpret_cast<wlr_seat_request_set_primary_selection_event*>(data));
}

void QWSeatPrivate::on_set_primary_selection(void *data)
{
    Q_ASSERT(m_handle == data);
    Q_EMIT q_func()->primarySelectionChanged();
}

void QWSeatPrivate::on_request_start_drag(void *data)
{
    Q_EMIT q_func()->requestStartDrag(reinterpret_cast<wlr_seat_request_start_drag_event*>(data));
}

void QWSeatPrivate::on_start_drag(void *data)
{
    Q_EMIT q_func()->startDrag(reinterpret_cast<wlr_drag*>(data));
}

QWSeat::QWSeat(wlr_seat *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWSeatPrivate(handle, isOwner, this))
{

}

QWSeat *QWSeat::create(QWDisplay *display, const char *name)
{
    auto handle = wlr_seat_create(display->handle(), name);
    if (!handle)
        return nullptr;
    return new QWSeat(handle, true);
}


QWSeat *QWSeat::get(wlr_seat *handle)
{
    return QWSeatPrivate::map.value(handle);
}

QWSeat *QWSeat::from(wlr_seat *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWSeat(handle, false);
}

void QWSeat::setKeyboard(wlr_keyboard *keyboard)
{
    wlr_seat_set_keyboard(handle(), keyboard);
}

wlr_keyboard *QWSeat::getKeyboard() const
{
    return wlr_seat_get_keyboard(handle());
}

void QWSeat::setCapabilities(uint32_t capabilities)
{
    wlr_seat_set_capabilities(handle(), capabilities);
}

void QWSeat::setSelection(wlr_data_source *source, uint32_t serial)
{
    wlr_seat_set_selection(handle(), source, serial);
}

void QWSeat::keyboardNotifyEnter(wlr_surface *surface, uint32_t keycodes[], size_t num_keycodes, wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_notify_enter(handle(), surface, keycodes, num_keycodes, modifiers);
}

void QWSeat::keyboardNotifyKey(uint32_t time_msec, uint32_t key, uint32_t state)
{
    wlr_seat_keyboard_notify_key(handle() ,time_msec, key, state);
}

void QWSeat::keyboardNotifyModifiers(wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_notify_modifiers(handle(), modifiers);
}

void QWSeat::pointerNotifyAxis(uint32_t time_msec, wlr_axis_orientation_t orientation, double value, int32_t value_discrete, wlr_axis_source_t source)
{
    wlr_seat_pointer_notify_axis(handle(), time_msec, static_cast<wlr_axis_orientation>(orientation), value, value_discrete, static_cast<wlr_axis_source>(source));
}

void QWSeat::pointerNotifyButton(uint32_t time_msec, uint32_t button, wlr_button_state_t state)
{
    wlr_seat_pointer_notify_button(handle(), time_msec, button, static_cast<wlr_button_state>(state));
}

void QWSeat::pointerNotifyEnter(wlr_surface *surface, double sx, double sy)
{
    wlr_seat_pointer_notify_enter(handle(), surface, sx, sy);
}

void QWSeat::pointerNotifyFrame()
{
    wlr_seat_pointer_notify_frame(handle());
}

void QWSeat::pointerNotifyMotion(uint32_t time_msec, double sx, double sy)
{
    wlr_seat_pointer_notify_motion(handle(), time_msec, sx, sy);
}

void QWSeat::pointerClearFocus()
{
    wlr_seat_pointer_clear_focus(handle());
}

QW_END_NAMESPACE
