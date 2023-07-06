// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcursor.h"
#include "qwbuffer.h"
#include "qwinputdevice.h"
#include "qwcompositor.h"
#include "util/qwsignalconnector.h"
#include "qwoutputlayout.h"
#include "qwoutput.h"
#include "qwxcursormanager.h"

#include <QImage>
#include <QPointF>
#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_cursor.h>
#undef static
#include <wlr/util/box.h>
}

QW_BEGIN_NAMESPACE

class QWCursorPrivate : public QWObjectPrivate
{
public:
    QWCursorPrivate(wlr_cursor *handle, bool isOwner, QWCursor *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.motion, this, &QWCursorPrivate::on_motion);
        sc.connect(&handle->events.motion_absolute, this, &QWCursorPrivate::on_motion_absolute);
        sc.connect(&handle->events.button, this, &QWCursorPrivate::on_button);
        sc.connect(&handle->events.axis, this, &QWCursorPrivate::on_axis);
        sc.connect(&handle->events.frame, this, &QWCursorPrivate::on_frame);
        sc.connect(&handle->events.swipe_begin, this, &QWCursorPrivate::on_swipe_begin);
        sc.connect(&handle->events.swipe_update, this, &QWCursorPrivate::on_swipe_update);
        sc.connect(&handle->events.swipe_end, this, &QWCursorPrivate::on_swipe_end);
        sc.connect(&handle->events.pinch_begin, this, &QWCursorPrivate::on_pinch_begin);
        sc.connect(&handle->events.pinch_update, this, &QWCursorPrivate::on_pinch_update);
        sc.connect(&handle->events.pinch_end, this, &QWCursorPrivate::on_pinch_end);
        sc.connect(&handle->events.hold_begin, this, &QWCursorPrivate::on_hold_begin);
        sc.connect(&handle->events.hold_end, this, &QWCursorPrivate::on_hold_end);
        sc.connect(&handle->events.touch_up, this, &QWCursorPrivate::on_touch_up);
        sc.connect(&handle->events.touch_down, this, &QWCursorPrivate::on_touch_down);
        sc.connect(&handle->events.touch_motion, this, &QWCursorPrivate::on_touch_motion);
        sc.connect(&handle->events.touch_cancel, this, &QWCursorPrivate::on_touch_cancel);
        sc.connect(&handle->events.touch_frame, this, &QWCursorPrivate::on_touch_frame);
        sc.connect(&handle->events.tablet_tool_axis, this, &QWCursorPrivate::on_tablet_tool_axis);
        sc.connect(&handle->events.tablet_tool_proximity, this, &QWCursorPrivate::on_tablet_tool_proximity);
        sc.connect(&handle->events.tablet_tool_tip, this, &QWCursorPrivate::on_tablet_tool_tip);
        sc.connect(&handle->events.tablet_tool_button, this, &QWCursorPrivate::on_tablet_tool_button);
    }
    ~QWCursorPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_cursor_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_motion(void *data);
    void on_motion_absolute(void *data);
    void on_button(void *data);
    void on_axis(void *data);
    void on_frame(void *data);
    void on_swipe_begin(void *data);
    void on_swipe_update(void *data);
    void on_swipe_end(void *data);
    void on_pinch_begin(void *data);
    void on_pinch_update(void *data);
    void on_pinch_end(void *data);
    void on_hold_begin(void *data);
    void on_hold_end(void *data);
    void on_touch_up(void *data);
    void on_touch_down(void *data);
    void on_touch_motion(void *data);
    void on_touch_cancel(void *data);
    void on_touch_frame(void *);
    void on_tablet_tool_axis(void *data);
    void on_tablet_tool_proximity(void *data);
    void on_tablet_tool_tip(void *data);
    void on_tablet_tool_button(void *data);

    static QHash<void*, QWCursor*> map;
    QW_DECLARE_PUBLIC(QWCursor)
    QWSignalConnector sc;
};
QHash<void*, QWCursor*> QWCursorPrivate::map;

void QWCursorPrivate::on_motion(void *data)
{
    Q_EMIT q_func()->motion(reinterpret_cast<wlr_pointer_motion_event*>(data));
}

void QWCursorPrivate::on_motion_absolute(void *data)
{
    Q_EMIT q_func()->motionAbsolute(reinterpret_cast<wlr_pointer_motion_absolute_event*>(data));
}

void QWCursorPrivate::on_button(void *data)
{
    Q_EMIT q_func()->button(reinterpret_cast<wlr_pointer_button_event*>(data));
}

void QWCursorPrivate::on_axis(void *data)
{
    Q_EMIT q_func()->axis(reinterpret_cast<wlr_pointer_axis_event*>(data));
}

void QWCursorPrivate::on_frame(void *data)
{
    Q_ASSERT(data == m_handle);
    Q_EMIT q_func()->frame();
}

void QWCursorPrivate::on_swipe_begin(void *data)
{
    Q_EMIT q_func()->swipeBegin(reinterpret_cast<wlr_pointer_swipe_begin_event*>(data));
}

void QWCursorPrivate::on_swipe_update(void *data)
{
    Q_EMIT q_func()->swipeUpdate(reinterpret_cast<wlr_pointer_swipe_update_event*>(data));
}

void QWCursorPrivate::on_swipe_end(void *data)
{
    Q_EMIT q_func()->swipeEnd(reinterpret_cast<wlr_pointer_swipe_end_event*>(data));
}

void QWCursorPrivate::on_pinch_begin(void *data)
{
    Q_EMIT q_func()->pinchBegin(reinterpret_cast<wlr_pointer_pinch_begin_event*>(data));
}

void QWCursorPrivate::on_pinch_update(void *data)
{
    Q_EMIT q_func()->pinchUpdate(reinterpret_cast<wlr_pointer_pinch_update_event*>(data));
}

void QWCursorPrivate::on_pinch_end(void *data)
{
    Q_EMIT q_func()->pinchEnd(reinterpret_cast<wlr_pointer_pinch_end_event*>(data));
}

void QWCursorPrivate::on_hold_begin(void *data)
{
    Q_EMIT q_func()->holdBegin(reinterpret_cast<wlr_pointer_hold_begin_event*>(data));
}

void QWCursorPrivate::on_hold_end(void *data)
{
    Q_EMIT q_func()->holdEnd(reinterpret_cast<wlr_pointer_hold_end_event*>(data));
}

void QWCursorPrivate::on_touch_up(void *data)
{
    Q_EMIT q_func()->touchUp(reinterpret_cast<wlr_touch_up_event*>(data));
}

void QWCursorPrivate::on_touch_down(void *data)
{
    Q_EMIT q_func()->touchDown(reinterpret_cast<wlr_touch_down_event*>(data));
}

void QWCursorPrivate::on_touch_motion(void *data)
{
    Q_EMIT q_func()->touchMotion(reinterpret_cast<wlr_touch_motion_event*>(data));
}

void QWCursorPrivate::on_touch_cancel(void *data)
{
    Q_EMIT q_func()->touchCancel(reinterpret_cast<wlr_touch_cancel_event*>(data));
}

void QWCursorPrivate::on_touch_frame(void *)
{
    Q_EMIT q_func()->touchFrame();
}

void QWCursorPrivate::on_tablet_tool_axis(void *data)
{
    Q_EMIT q_func()->tabletToolAxis(reinterpret_cast<wlr_tablet_tool_axis_event*>(data));
}

void QWCursorPrivate::on_tablet_tool_proximity(void *data)
{
    Q_EMIT q_func()->tabletToolProximity(reinterpret_cast<wlr_tablet_tool_proximity_event*>(data));
}

void QWCursorPrivate::on_tablet_tool_tip(void *data)
{
    Q_EMIT q_func()->tabletToolTip(reinterpret_cast<wlr_tablet_tool_tip_event*>(data));
}

void QWCursorPrivate::on_tablet_tool_button(void *data)
{
    Q_EMIT q_func()->tabletToolButton(reinterpret_cast<wlr_tablet_tool_button*>(data));
}

QWCursor::QWCursor(wlr_cursor *handle, bool isOwner, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWCursorPrivate(handle, isOwner, this))
{

}

QWCursor::QWCursor(QObject *parent)
    : QWCursor(wlr_cursor_create(), true, parent)
{

}

QWCursor *QWCursor::get(wlr_cursor *handle)
{
    return QWCursorPrivate::map.value(handle);
}

QWCursor *QWCursor::from(wlr_cursor *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWCursor(handle, false, nullptr);
}

bool QWCursor::warp(QWInputDevice *dev, const QPointF &pos)
{
    return wlr_cursor_warp(handle(), dev->handle(), pos.x(), pos.y());
}

void QWCursor::warpClosest(QWInputDevice *dev, const QPointF &pos)
{
    wlr_cursor_warp_closest(handle(), dev->handle(), pos.x(), pos.y());
}

void QWCursor::warpAbsolute(QWInputDevice *dev, const QPointF &pos)
{
    wlr_cursor_warp_absolute(handle(), dev->handle(), pos.x(), pos.y());
}

void QWCursor::move(QWInputDevice *dev, const QPointF &deltaPos)
{
    wlr_cursor_move(handle(), dev ? dev->handle() : nullptr, deltaPos.x(), deltaPos.y());
}

#if WLR_VERSION_MINOR > 16
void QWCursor::setBuffer(QWBuffer *buffer, const QPoint &hotspot, float scale)
{
    wlr_cursor_set_buffer(handle(), buffer->handle(),
                         hotspot.x(), hotspot.y(), scale);
}

void QWCursor::setXCursor(QWXCursorManager *manager, const char *name)
{
    wlr_cursor_set_xcursor(handle(), manager->handle(), name);
}
#else
void QWCursor::setImage(const QImage &image, const QPoint &hotspot)
{
    wlr_cursor_set_image(handle(), reinterpret_cast<const uint8_t*>(image.constBits()),
                         image.bytesPerLine(), image.width(), image.height(),
                         hotspot.x(), hotspot.y(), image.devicePixelRatioF());
}
#endif

void QWCursor::setSurface(QWSurface *surface, const QPoint &hotspot)
{
    wlr_cursor_set_surface(handle(), surface->handle(), hotspot.x(), hotspot.y());
}

void QWCursor::attachInputDevice(QWInputDevice *dev)
{
    wlr_cursor_attach_input_device(handle(), dev->handle());
}

void QWCursor::detachInputDevice(QWInputDevice *dev)
{
    wlr_cursor_detach_input_device(handle(), dev->handle());
}

void QWCursor::attachOutputLayout(QWOutputLayout *layout)
{
    wlr_cursor_attach_output_layout(handle(), layout->handle());
}

void QWCursor::mapToOutput(QWOutput *output)
{
    wlr_cursor_map_to_output(handle(), output->handle());
}

void QWCursor::mapInputToOutput(QWInputDevice *dev, QWOutput *output)
{
    wlr_cursor_map_input_to_output(handle(), dev->handle(), output->handle());
}

void QWCursor::mapToRegion(const QRect &box)
{
    wlr_box b {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };

    wlr_cursor_map_to_region(handle(), &b);
}

void QWCursor::mapInputToRegion(QWInputDevice *dev, const QRect &box)
{
    wlr_box b {
        .x = box.x(),
        .y = box.y(),
        .width = box.width(),
        .height = box.height()
    };

    wlr_cursor_map_input_to_region(handle(), dev->handle(), &b);
}

QPointF QWCursor::position() const
{
    return QPointF(handle()->x, handle()->y);
}

QW_END_NAMESPACE
