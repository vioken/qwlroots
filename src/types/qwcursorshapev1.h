// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_cursor_shape_manager_v1;
struct wlr_cursor_shape_manager_v1_request_set_shape_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWCursorShapeManagerV1Private;

class QW_EXPORT QWCursorShapeManagerV1 : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWCursorShapeManagerV1)
public:
    inline wlr_cursor_shape_manager_v1 *handle() const {
        return QWObject::handle<wlr_cursor_shape_manager_v1>();
    }

    static QWCursorShapeManagerV1 *get(wlr_cursor_shape_manager_v1 *handle);
    static QWCursorShapeManagerV1 *from(wlr_cursor_shape_manager_v1 *handle);
    static QWCursorShapeManagerV1 *create(QWDisplay *display, uint32_t version);

Q_SIGNALS:
    void requestSetShape(wlr_cursor_shape_manager_v1_request_set_shape_event *event);

private:
    QWCursorShapeManagerV1(wlr_cursor_shape_manager_v1 *handle, bool isOwner);
    ~QWCursorShapeManagerV1() = default;
};

QW_END_NAMESPACE
