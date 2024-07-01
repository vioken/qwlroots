// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_data_device.h>
}


QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(data_source) {
    QW_OBJECT
    Q_OBJECT
public:
    QW_FUNC_MEMBER(data_source, destroy);
    QW_FUNC_MEMBER(data_source, init);
    QW_FUNC_MEMBER(data_source, accept);
    QW_FUNC_MEMBER(data_source, send);
    QW_FUNC_MEMBER(data_source, dnd_action);
    QW_FUNC_MEMBER(data_source, dnd_drop);
    QW_FUNC_MEMBER(data_source, dnd_finish);
};

class QW_CLASS_OBJECT(data_device_manager) {
    QW_OBJECT
    Q_OBJECT
public:
    QW_FUNC_STATIC(data_device_manager, create);
};

class QW_CLASS_OBJECT(drag) {
    QW_OBJECT
    Q_OBJECT
    QW_SIGNAL(focus)
    QW_SIGNAL(motion, wlr_drag_motion_event*)
    QW_SIGNAL(drop, wlr_drag_drop_event*)
public:
    QW_FUNC_STATIC(drag, create);
    QW_FUNC_STATIC(seat, request_start_drag);
    QW_FUNC_STATIC(seat, start_drag);
};

QW_END_NAMESPACE
