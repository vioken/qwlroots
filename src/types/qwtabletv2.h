// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

typedef uint32_t zwp_tablet_pad_v2_button_state_t;

struct wlr_tablet_tool;
struct wlr_tablet_pad_v2_grab;
struct wlr_tablet_v2_tablet_tool;
struct wlr_tablet_manager_v2;
struct wlr_tablet_v2_event_cursor;
struct wlr_tablet_v2_tablet;
struct wlr_tablet_tool_v2_grab;
struct wlr_tablet_v2_tablet_pad;
struct wlr_tablet_v2_event_feedback;

QT_BEGIN_NAMESPACE

class QPointF;

QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QWInputDevice;
class QWTabletTool;
class QW_EXPORT QWTabletPadV2Grab
{
public:
    QWTabletPadV2Grab() = delete;
    ~QWTabletPadV2Grab() = delete;
    static QWTabletPadV2Grab *from(wlr_tablet_pad_v2_grab *handle);
    wlr_tablet_pad_v2_grab *handle() const;
};

class QW_EXPORT QWTabletToolV2Grab
{
public:
    QWTabletToolV2Grab() = delete;
    ~QWTabletToolV2Grab() = delete;
    static QWTabletToolV2Grab *from(wlr_tablet_tool_v2_grab *handle);
    wlr_tablet_tool_v2_grab *handle() const;
};

class QWSurface;
class QWTabletV2TabletPrivate;
class QW_EXPORT QWTabletV2Tablet : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletV2Tablet)

    friend class QWTabletManagerV2;
public:
    inline wlr_tablet_v2_tablet *handle() const {
        return QWObject::handle<wlr_tablet_v2_tablet>();
    }

    static QWTabletV2Tablet *get(wlr_tablet_v2_tablet *handle);
    static QWTabletV2Tablet *from(wlr_tablet_v2_tablet *handle);

    bool canAcceptTablet(QWSurface *surface) const;

Q_SIGNALS:
    void beforeDestroy(QWTabletV2Tablet *self);

private:
    explicit QWTabletV2Tablet(wlr_tablet_v2_tablet *handle, bool isOwner, QWInputDevice *parent);
    ~QWTabletV2Tablet() = default;
};

class QWSurface;
class QWTabletV2TabletToolPrivate;
class QW_EXPORT QWTabletV2TabletTool : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletV2TabletTool)

    friend class QWTabletManagerV2;
public:
    static QWTabletV2TabletTool *get(wlr_tablet_v2_tablet_tool *handle);
    static QWTabletV2TabletTool *from(wlr_tablet_v2_tablet_tool *handle);

    inline wlr_tablet_v2_tablet_tool *handle() const {
        return QWObject::handle<wlr_tablet_v2_tablet_tool>();
    }

    void sendProximityIn(QWTabletV2Tablet *tablet, QWSurface *surface);
    void sendDown();
    void sendUp();
    void sendMotion(const QPointF &pos);
    void sendPressure(double pressure);
    void sendDistance(double distance);
    void sendTilt(const QPointF &pos);
    void sendRotation(double degrees);
    void sendSlider(double position);
    void sendWheel(double degrees, int32_t clicks);
    void sendProximityOut();
    void sendButton(uint32_t button, zwp_tablet_pad_v2_button_state_t state);
    void notifyProximityIn(QWTabletV2Tablet *tablet, QWSurface *surface);
    void notifyDown();
    void notifyUp();
    void notifyMotion(const QPointF &pos);
    void notifyPressure(double pressure);
    void notifyDistance(double distance);
    void notifyTilt(const QPointF &pos);
    void notifyRotation(double degrees);
    void notifySlider(double position);
    void notifyWheel(double degrees, int32_t clicks);
    void notifyProximityOut();
    void notifyButton(uint32_t button, zwp_tablet_pad_v2_button_state_t state);
    void startGrab(QWTabletToolV2Grab *grab);
    void endGrab();
    void startImplicitGrab();
    bool hasImplicitGrab() const;

Q_SIGNALS:
    void beforeDestroy(QWTabletV2TabletTool *self);
    void setCursor(wlr_tablet_v2_event_cursor *);

private:
    QWTabletV2TabletTool(wlr_tablet_v2_tablet_tool *handle, bool isOwner, QWTabletTool *parent);
    ~QWTabletV2TabletTool() = default;
};

class QWTabletV2TabletPadPrivate;
class QW_EXPORT QWTabletV2TabletPad : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletV2TabletPad)

    friend class QWTabletManagerV2;
public:
    inline wlr_tablet_v2_tablet_pad *handle() const {
        return QWObject::handle<wlr_tablet_v2_tablet_pad>();
    }

    static QWTabletV2TabletPad *get(wlr_tablet_v2_tablet_pad *handle);
    static QWTabletV2TabletPad *from(wlr_tablet_v2_tablet_pad *handle);

    uint32_t sendEnter(QWTabletV2Tablet *tablet, QWSurface *surface);
    void sendButton(size_t button, uint32_t time, zwp_tablet_pad_v2_button_state_t state);
    void sendStrip(uint32_t strip, double position, bool finger, uint32_t time);
    void sendRing(uint32_t ring, double position, bool finger, uint32_t time);
    uint32_t sendLeave(QWSurface *surface);
    uint32_t sendMode(size_t group, uint32_t mode, uint32_t time);
    uint32_t notifyEnter(QWTabletV2Tablet *tablet, QWSurface *surface);
    void notifyButton(size_t button, uint32_t time, zwp_tablet_pad_v2_button_state_t state);
    void notifyStrip(uint32_t strip, double position, bool finger, uint32_t time);
    void notifyRing(uint32_t ring, double position, bool finger, uint32_t time);
    uint32_t notifyLeave(QWSurface *surface);
    uint32_t notifyMode(size_t group, uint32_t mode, uint32_t time);
    void endGrab();
    void startGrab(QWTabletPadV2Grab *grab);

Q_SIGNALS:
    void beforeDestroy(QWTabletV2TabletPad *self);
    void buttonFeedback(wlr_tablet_v2_event_feedback *);
    void ringFeedback(wlr_tablet_v2_event_feedback *);
    void stripFeedback(wlr_tablet_v2_event_feedback *);

private:
    QWTabletV2TabletPad(wlr_tablet_v2_tablet_pad *handle, bool isOwner, QWInputDevice *parent);
    ~QWTabletV2TabletPad() = default;
};

class QWDisplay;
class QWSeat;
class QWInputDevice;
class QWTabletManagerV2Private;
class QW_EXPORT QWTabletManagerV2 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletManagerV2)
public:
    inline wlr_tablet_manager_v2 *handle() const {
        return QWObject::handle<wlr_tablet_manager_v2>();
    }

    static QWTabletManagerV2 *get(wlr_tablet_manager_v2 *handle);
    static QWTabletManagerV2 *from(wlr_tablet_manager_v2 *handle);
    static QWTabletManagerV2 *create(QWDisplay *display);

    QWTabletV2Tablet *createTablet(QWSeat *wlr_seat, QWInputDevice *wlr_device);
    QWTabletV2TabletPad *createPad(QWSeat *wlr_seat, QWInputDevice *wlr_device);
    QWTabletV2TabletTool *createTool(QWSeat *wlr_seat, wlr_tablet_tool *wlr_tool);

Q_SIGNALS:
    void beforeDestroy(QWTabletManagerV2 *self);

private:
    QWTabletManagerV2(wlr_tablet_manager_v2 *handle, bool isOwner);
    ~QWTabletManagerV2() = default;
};

QW_END_NAMESPACE
