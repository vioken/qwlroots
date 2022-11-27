// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_cursor;
struct wlr_input_device;
struct wlr_surface;
struct wlr_output_layout;
struct wlr_output;
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
struct wlr_touch_up_event;
struct wlr_touch_down_event;
struct wlr_touch_motion_event;
struct wlr_touch_cancel_event;
struct wlr_tablet_tool_axis_event;
struct wlr_tablet_tool_proximity_event;
struct wlr_tablet_tool_tip_event;
struct wlr_tablet_tool_button;

QW_BEGIN_NAMESPACE

class QWOutputLayout;
class QWCursorPrivate;
class QW_EXPORT QWCursor : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWCursor)
public:
    explicit QWCursor(wlr_cursor *handle);

    inline wlr_cursor *handle() const {
        return QWObject::handle<wlr_cursor>();
    }

    static QWCursor *create();

    bool warp(wlr_input_device *dev, const QPointF &pos);
    void warpClosest(wlr_input_device *dev, const QPointF &pos);
    void warpAbsolute(wlr_input_device *dev, const QPointF &pos);
    void move(wlr_input_device *dev, const QPointF &deltaPos);
    void setImage(const QImage &image, const QPoint &hotspot);
    void setSurface(wlr_surface *surface, const QPoint &hotspot);

    void attachInputDevice(wlr_input_device *dev);
    void detachInputDevice(wlr_input_device *dev);
    void attachOutputLayout(QWOutputLayout *layout);
    void mapToOutput(wlr_output *output);
    void mapInputToOutput(wlr_input_device *dev, wlr_output *output);
    void mapToRegion(const QRect &box);
    void mapInputToRegion(wlr_input_device *dev, const QRect &box);

    QPointF absoluteToLayoutCoords(wlr_input_device *dev, const QPointF &pos) const;

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
    void touchUp(wlr_touch_up_event *event);
    void touchDown(wlr_touch_down_event *event);
    void touchMotion(wlr_touch_motion_event *event);
    void touchCancel(wlr_touch_cancel_event *event);
    void touchFrame();
    void tabletToolAxis(wlr_tablet_tool_axis_event *event);
    void tabletToolProximity(wlr_tablet_tool_proximity_event *event);
    void tabletToolTip(wlr_tablet_tool_tip_event *event);
    void tabletToolButton(wlr_tablet_tool_button *event);
};

QW_END_NAMESPACE
