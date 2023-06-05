// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_pointer;
struct wlr_pointer_impl;
struct wlr_pointer_motion_event;
struct wlr_pointer_motion_absolute_event;
struct wlr_pointer_button_event;
struct wlr_pointer_axis_event;
struct wlr_pointer_swipe_begin_event;
struct wlr_pointer_swipe_update_event;
struct wlr_pointer_swipe_end_event;
struct wlr_pointer_pinch_begin_event;
struct wlr_pointer_pinch_update_event;
struct wlr_pointer_pinch_end_event;
struct wlr_pointer_hold_begin_event;
struct wlr_pointer_hold_end_event;

QW_BEGIN_NAMESPACE

class QWPointerInterface : public QWInterface {
    friend class QWPointer;
public:
    virtual const char* name() = 0;

    inline wlr_pointer *handle() const {
        return QWInterface::handle<wlr_pointer>();
    }

protected:
    void notifyMotion(wlr_pointer_motion_event *event);
    void notifyMotionAbsolute(wlr_pointer_motion_absolute_event *event);
    void notifyButton(wlr_pointer_button_event *event);
    void notifyAxis(wlr_pointer_axis_event *event);
    void notifyFrame();
    void notifySwipeBegin(wlr_pointer_swipe_begin_event *event);
    void notifySwipeUpdate(wlr_pointer_swipe_update_event *event);
    void notifySwipeEnd(wlr_pointer_swipe_end_event *event);
    void notifyPinchBegin(wlr_pointer_pinch_begin_event *event);
    void notifyPinchUpdate(wlr_pointer_pinch_update_event *event);
    void notifyPinchEnd(wlr_pointer_pinch_end_event *event);
    void notifyHoldBegin(wlr_pointer_hold_begin_event *event);
    void notifyHoldEnd(wlr_pointer_hold_end_event *event);

    template<class T>
    inline void init(const char* name)
    {
        init(getFuncMagicKey<T>(), name);
    }

    virtual void init(FuncMagicKey funMagicKey, const char* name);
};

QW_END_NAMESPACE
