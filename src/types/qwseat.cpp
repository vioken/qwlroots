// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwseat.h"
#include "qwdisplay.h"
#include "qwkeyboard.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_pointer.h>
}

static_assert(std::is_same_v<wlr_axis_orientation_t, std::underlying_type_t<wlr_axis_orientation>>);
static_assert(std::is_same_v<wlr_axis_source_t, std::underlying_type_t<wlr_axis_source>>);
static_assert(std::is_same_v<wlr_button_state_t, std::underlying_type_t<wlr_button_state>>);

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
        Q_EMIT q_func()->beforeDestroy(q_func());
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

void QWSeatPrivate::on_pointer_grab_begin(void *)
{
    Q_EMIT q_func()->pointerGrabBegin();
}

void QWSeatPrivate::on_pointer_grab_end(void *)
{
    Q_EMIT q_func()->pointerGrabEnd();
}

void QWSeatPrivate::on_touch_grab_begin(void *)
{
    Q_EMIT q_func()->touchGrabBegin();
}

void QWSeatPrivate::on_touch_grab_end(void *)
{
    Q_EMIT q_func()->touchGrabEnd();
}

void QWSeatPrivate::on_keyboard_grab_begin(void *)
{
    Q_EMIT q_func()->keyboardGrabBegin();
}

void QWSeatPrivate::on_keyboard_grab_end(void *)
{
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

void QWSeatPrivate::on_set_selection(void *)
{
    Q_EMIT q_func()->selectionChanged();
}

void QWSeatPrivate::on_request_set_primary_selection(void *data)
{
    Q_EMIT q_func()->requestSetPrimarySelection(reinterpret_cast<wlr_seat_request_set_primary_selection_event*>(data));
}

void QWSeatPrivate::on_set_primary_selection(void *)
{
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

QWSeat *QWSeat::get(wlr_seat *handle)
{
    return QWSeatPrivate::map.value(handle);
}

QWSeat *QWSeat::from(wlr_seat *handle)
{
    if (auto *o = get(handle))
        return o;
    return new QWSeat(handle, false);
}

QWSeat *QWSeat::create(QWDisplay *display, const char *name)
{
    auto *handle = wlr_seat_create(display->handle(), name);
    if (!handle)
        return nullptr;
    return new QWSeat(handle, true);
}

void QWSeat::setKeyboard(QWKeyboard *keyboard)
{
    wlr_seat_set_keyboard(handle(), keyboard->handle());
}

QWKeyboard *QWSeat::getKeyboard() const
{
    return QWKeyboard::from(wlr_seat_get_keyboard(handle()));
}

void QWSeat::setCapabilities(uint32_t capabilities)
{
    wlr_seat_set_capabilities(handle(), capabilities);
}

void QWSeat::setSelection(wlr_data_source *source, uint32_t serial)
{
    wlr_seat_set_selection(handle(), source, serial);
}

void QWSeat::keyboardClearFocus()
{
    wlr_seat_keyboard_clear_focus(handle());
}

void QWSeat::keyboardEndGrab()
{
    wlr_seat_keyboard_end_grab(handle());
}

#if WLR_VERSION_MINOR > 16
void QWSeat::keyboardEnter(QWSurface *surface, const uint32_t *keycodes, size_t numKeycodes, const wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_enter(handle(), surface->handle(), keycodes, numKeycodes, modifiers);
}
#else
void QWSeat::keyboardEnter(QWSurface *surface, uint32_t *keycodes, size_t numKeycodes, wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_enter(handle(), surface->handle(), keycodes, numKeycodes, modifiers);
}
#endif

bool QWSeat::keyboardHasGrab() const
{
    return wlr_seat_keyboard_has_grab(handle());
}

void QWSeat::keyboardNotifyClearFocus()
{
    wlr_seat_keyboard_notify_clear_focus(handle());
}

void QWSeat::keyboardNotifyEnter(QWSurface *surface, uint32_t keycodes[], size_t numKeycodes, wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_notify_enter(handle(), surface->handle(), keycodes, numKeycodes, modifiers);
}

void QWSeat::keyboardNotifyKey(uint32_t time_msec, uint32_t key, uint32_t state)
{
    wlr_seat_keyboard_notify_key(handle() ,time_msec, key, state);
}

void QWSeat::keyboardNotifyModifiers(wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_notify_modifiers(handle(), modifiers);
}

void QWSeat::keyboardSendKey(uint32_t timeMsec, uint32_t key, uint32_t state)
{
    wlr_seat_keyboard_send_key(handle(), timeMsec, key, state);
}

void QWSeat::keyboardSendModifiers(wlr_keyboard_modifiers *modifiers)
{
    wlr_seat_keyboard_send_modifiers(handle(), modifiers);
}

void QWSeat::keyboardStartGrab(wlr_seat_keyboard_grab *grab)
{
    wlr_seat_keyboard_start_grab(handle(), grab);
}

void QWSeat::pointerEndGrab()
{
    wlr_seat_pointer_end_grab(handle());
}

void QWSeat::pointerEnter(QWSurface *surface, double sx, double sy)
{
    wlr_seat_pointer_enter(handle(), surface->handle(), sx, sy);
}

bool QWSeat::pointerHasGrab() const
{
    return wlr_seat_pointer_has_grab(handle());
}

void QWSeat::pointerNotifyAxis(uint32_t time_msec, wlr_axis_orientation_t orientation, double value, int32_t value_discrete, wlr_axis_source_t source)
{
    wlr_seat_pointer_notify_axis(handle(), time_msec, static_cast<wlr_axis_orientation>(orientation), value, value_discrete, static_cast<wlr_axis_source>(source));
}

void QWSeat::pointerNotifyButton(uint32_t time_msec, uint32_t button, wlr_button_state_t state)
{
    wlr_seat_pointer_notify_button(handle(), time_msec, button, static_cast<wlr_button_state>(state));
}

void QWSeat::pointerNotifyClearFocus()
{
    wlr_seat_pointer_notify_clear_focus(handle());
}

void QWSeat::pointerNotifyEnter(QWSurface *surface, double sx, double sy)
{
    wlr_seat_pointer_notify_enter(handle(), surface->handle(), sx, sy);
}

void QWSeat::pointerNotifyFrame()
{
    wlr_seat_pointer_notify_frame(handle());
}

void QWSeat::pointerNotifyMotion(uint32_t time_msec, double sx, double sy)
{
    wlr_seat_pointer_notify_motion(handle(), time_msec, sx, sy);
}

void QWSeat::pointerSendAxis(uint32_t timeMsec, wlr_axis_orientation_t orientation, double value, int32_t valueDiscrete, wlr_axis_source_t source)
{
    wlr_seat_pointer_send_axis(handle(), timeMsec, static_cast<wlr_axis_orientation>(orientation), value, valueDiscrete, static_cast<wlr_axis_source>(source));
}

uint32_t QWSeat::pointerSendButton(uint32_t timeMsec, uint32_t button, wlr_button_state_t state)
{
    return wlr_seat_pointer_send_button(handle(), timeMsec, button, static_cast<wlr_button_state>(state));
}

void QWSeat::pointerSendFrame()
{
    wlr_seat_pointer_send_frame(handle());
}

void QWSeat::pointerSendMotion(uint32_t timeMsec, double sx, double sy)
{
    wlr_seat_pointer_send_motion(handle(), timeMsec, sx, sy);
}

void QWSeat::pointerStartGrab(wlr_seat_pointer_grab *grab)
{
    wlr_seat_pointer_start_grab(handle(), grab);
}

bool QWSeat::pointerSurfaceHasFocus(QWSurface *surface) const
{
    return wlr_seat_pointer_surface_has_focus(handle(), surface->handle());
}

void QWSeat::pointerWarp(double sx, double sy)
{
    wlr_seat_pointer_warp(handle(), sx, sy);
}

void QWSeat::pointerClearFocus()
{
    wlr_seat_pointer_clear_focus(handle());
}

void QWSeat::setName(const char* name)
{
    wlr_seat_set_name(handle(), name);
}

void QWSeat::touchEndGrab()
{
    wlr_seat_touch_end_grab(handle());
}

wlr_touch_point *QWSeat::touchGetPoint(int32_t touchId) const
{
    return wlr_seat_touch_get_point(handle(), touchId);
}

bool QWSeat::touchHasGrab() const
{
    return wlr_seat_touch_has_grab(handle());
}

void QWSeat::touchNotifyCancel(QWSurface *surface)
{
    wlr_seat_touch_notify_cancel(handle(), surface->handle());
}

uint32_t QWSeat::touchNotifyDown(QWSurface *surface, uint32_t timeMsec, int32_t touch_id, double sx, double sy)
{
    return wlr_seat_touch_notify_down(handle(), surface->handle(), timeMsec, touch_id, sx, sy);
}

void QWSeat::touchNotifyFrame()
{
    wlr_seat_touch_notify_frame(handle());
}

void QWSeat::touchNotifyMotion(uint32_t timeMsec, int32_t touchId, double sx, double sy)
{
    wlr_seat_touch_notify_motion(handle(), timeMsec, touchId, sx, sy);
}

void QWSeat::touchNotifyUp(uint32_t timeMsec, int32_t touchId)
{
    wlr_seat_touch_notify_up(handle(), timeMsec, touchId);
}

int QWSeat::touchNumPoints()
{
    return wlr_seat_touch_num_points(handle());
}

void QWSeat::touchPointClearFocus(uint32_t timeMsec, int32_t touchId)
{
    wlr_seat_touch_point_clear_focus(handle(), timeMsec, touchId);
}

void QWSeat::touchPointFocus(QWSurface *surface, uint32_t timeMsec, int32_t touchId, double sx, double sy)
{
    wlr_seat_touch_point_focus(handle(), surface->handle(), timeMsec, touchId, sx, sy);
}

void QWSeat::touchSendCancel(QWSurface *surface)
{
    wlr_seat_touch_send_cancel(handle(), surface->handle());
}

uint32_t QWSeat::touchSendDown(QWSurface *surface, uint32_t timeMsec, int32_t touchId, double sx, double sy)
{
    return wlr_seat_touch_send_down(handle(), surface->handle(), timeMsec, touchId, sx, sy);
}

void QWSeat::touchSendFrame()
{
    wlr_seat_touch_send_frame(handle());
}

void QWSeat::touchSendMotion(uint32_t timeMsec, int32_t touchId, double sx, double sy)
{
    wlr_seat_touch_send_motion(handle(), timeMsec, touchId, sx, sy);
}

void QWSeat::touchSendUp(uint32_t timeMsec, int32_t touchId)
{
    wlr_seat_touch_send_up(handle(), timeMsec, touchId);
}

void QWSeat::touchStartGrab(wlr_seat_touch_grab *grab)
{
    wlr_seat_touch_start_grab(handle(), grab);
}

bool QWSeat::validaiteGrabSerial(uint32_t serial)
{
    return wlr_seat_validate_grab_serial(handle(), serial);
}

bool QWSeat::validatePointerGrabSerial(QWSurface *origin, uint32_t serial)
{
    return wlr_seat_validate_grab_serial(handle(), serial);
}

bool QWSeat::validateTouchGrabSerial(QWSurface *origin, uint32_t serial, wlr_touch_point **pointPtr)
{
    return wlr_seat_validate_touch_grab_serial(handle(), origin->handle(), serial, pointPtr);
}

bool QWSeat::surfaceAcceptsTouch(QWSurface *surface)
{
    return wlr_surface_accepts_touch(handle(), surface->handle());
}

QW_END_NAMESPACE
