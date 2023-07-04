// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <QObject>

struct wlr_touch;
struct wlr_touch_down_event;
struct wlr_touch_up_event;
struct wlr_touch_motion_event;
struct wlr_touch_cancel_event;

QW_BEGIN_NAMESPACE

class QWTouchPrivate;

class QW_EXPORT QWTouch : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTouch)
public:
    inline wlr_touch *handle() const {
        return QWObject::handle<wlr_touch>();
    }

    static QWTouch *get(wlr_touch *handle);
    static QWTouch *from(wlr_touch *handle);
    static QWTouch *fromInputDevice(wlr_input_device *input_device);

Q_SIGNALS:
    void down(wlr_touch_down_event *event);
    void up(wlr_touch_up_event *event);
    void motion(wlr_touch_motion_event *event);
    void cancel(wlr_touch_cancel_event *event);
    void frame();

private:
    ~QWTouch() override = default;
    QWTouch(wlr_touch *handle, bool isOwner);
};

QW_END_NAMESPACE
