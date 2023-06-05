// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpointerinterface.h"

extern "C" {
#include <wlr/interfaces/wlr_pointer.h>
#include <wayland-server-core.h>
}

QW_BEGIN_NAMESPACE

struct _wlr_pointer : public wlr_pointer
{
    _wlr_pointer(QWPointerInterface *i)
        : interface(i)
    {}

    QWPointerInterface *interface;
};

void QWPointerInterface::notifyMotion(wlr_pointer_motion_event *event) {
    wl_signal_emit_mutable(&handle()->events.motion, event);
}

void QWPointerInterface::notifyMotionAbsolute(wlr_pointer_motion_absolute_event *event) {
    wl_signal_emit_mutable(&handle()->events.motion_absolute, event);
}

void QWPointerInterface::notifyButton(wlr_pointer_button_event *event) {
    wl_signal_emit_mutable(&handle()->events.button, event);
}

void QWPointerInterface::notifyAxis(wlr_pointer_axis_event *event) {
    wl_signal_emit_mutable(&handle()->events.axis, event);
}

void QWPointerInterface::notifyFrame() {
    wl_signal_emit_mutable(&handle()->events.frame, nullptr);
}

void QWPointerInterface::notifySwipeBegin(wlr_pointer_swipe_begin_event *event) {
    wl_signal_emit_mutable(&handle()->events.swipe_begin, event);
}

void QWPointerInterface::notifySwipeUpdate(wlr_pointer_swipe_update_event *event) {
    wl_signal_emit_mutable(&handle()->events.swipe_update, event);
}

void QWPointerInterface::notifySwipeEnd(wlr_pointer_swipe_end_event *event) {
    wl_signal_emit_mutable(&handle()->events.swipe_end, event);
}

void QWPointerInterface::notifyPinchBegin(wlr_pointer_pinch_begin_event *event) {
    wl_signal_emit_mutable(&handle()->events.pinch_begin, event);
}

void QWPointerInterface::notifyPinchUpdate(wlr_pointer_pinch_update_event *event) {
    wl_signal_emit_mutable(&handle()->events.pinch_update, event);
}

void QWPointerInterface::notifyPinchEnd(wlr_pointer_pinch_end_event *event) {
    wl_signal_emit_mutable(&handle()->events.pinch_end, event);
}

void QWPointerInterface::notifyHoldBegin(wlr_pointer_hold_begin_event *event) {
    wl_signal_emit_mutable(&handle()->events.hold_begin, event);
}

void QWPointerInterface::notifyHoldEnd(wlr_pointer_hold_end_event *event) {
    wl_signal_emit_mutable(&handle()->events.hold_end, event);
}

void QWPointerInterface::init(FuncMagicKey funMagicKey, const char* name)
{
    Q_UNUSED(funMagicKey);
    auto impl = new wlr_pointer_impl {
       .name = this->name()
    };
    m_handleImpl = impl;
    m_handle = calloc(1, sizeof(_wlr_pointer));
    static_cast<_wlr_pointer *>(m_handle)->interface = this;
    wlr_pointer_init(handle(), impl, name);
}

QW_END_NAMESPACE

