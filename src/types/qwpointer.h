// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <QObject>

struct wlr_pointer;
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

class QWPointerPrivate;
class QW_EXPORT QWPointer : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPointer)
public:
    inline wlr_pointer *handle() const {
        return QWObject::handle<wlr_pointer>();
    }

    static QWPointer *get(wlr_pointer *handle);
    static QWPointer *from(wlr_pointer *handle);
    static QWPointer *fromInputDevice(wlr_input_device *input_device);

Q_SIGNALS:
    void motion(wlr_pointer_motion_event *event);
    void motionAbsolute(wlr_pointer_motion_absolute_event *event);
    void button(wlr_pointer_button_event *event);
    void axis(wlr_pointer_axis_event *event);
    void frame();

    void swipeBegin(wlr_pointer_swipe_begin_event *event);
    void swipeUpdate(wlr_pointer_swipe_update_event *event);
    void swipeEnd(wlr_pointer_swipe_end_event *event);

    void pinchBegin(wlr_pointer_pinch_begin_event *event);
    void pinchUpdate(wlr_pointer_pinch_update_event *event);
    void pinchEnd(wlr_pointer_pinch_end_event *event);

    void holdBegin(wlr_pointer_hold_begin_event *event);
    void holdEnd(wlr_pointer_hold_end_event *event);

private:
    ~QWPointer() override = default;
    QWPointer(wlr_pointer *handle, bool isOwner);
};

QW_END_NAMESPACE
