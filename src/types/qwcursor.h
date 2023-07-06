// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_cursor;
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

class QWSurface;
class QWOutputLayout;
class QWCursorPrivate;
class QWInputDevice;
class QWOutput;
class QWBuffer;
class QWXCursorManager;

class QW_EXPORT QWCursor : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWCursor)
public:
    explicit QWCursor(QObject *parent = nullptr);
    ~QWCursor() = default;

    inline wlr_cursor *handle() const {
        return QWObject::handle<wlr_cursor>();
    }

    static QWCursor *get(wlr_cursor *handle);
    static QWCursor *from(wlr_cursor *handle);

    bool warp(QWInputDevice *dev, const QPointF &pos);
    void warpClosest(QWInputDevice *dev, const QPointF &pos);
    void warpAbsolute(QWInputDevice *dev, const QPointF &pos);
    void move(QWInputDevice *dev, const QPointF &deltaPos);
#if WLR_VERSION_MINOR > 16
    void setBuffer(QWBuffer *buffer, const QPoint &hotspot, float scale);
    void setXCursor(QWXCursorManager *manager, const char *name);
#else
    void setImage(const QImage &image, const QPoint &hotspot);
#endif
    void setSurface(QWSurface *surface, const QPoint &hotspot);

    void attachInputDevice(QWInputDevice *dev);
    void detachInputDevice(QWInputDevice *dev);
    void attachOutputLayout(QWOutputLayout *layout);
    void mapToOutput(QWOutput *output);
    void mapInputToOutput(QWInputDevice *dev, QWOutput *output);
    void mapToRegion(const QRect &box);
    void mapInputToRegion(QWInputDevice *dev, const QRect &box);

    QPointF absoluteToLayoutCoords(QWInputDevice *dev, const QPointF &pos) const;
    QPointF position() const;

Q_SIGNALS:
    void beforeDestroy(QWCursor *self);
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

private:
    QWCursor(wlr_cursor *handle, bool isOwner, QObject *parent);
};

QW_END_NAMESPACE
