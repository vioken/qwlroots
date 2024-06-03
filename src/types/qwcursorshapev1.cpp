// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcursorshapev1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_cursor_shape_v1.h>
}

QW_BEGIN_NAMESPACE

class QWCursorShapeManagerV1Private : public QWWrapObjectPrivate
{
public:
    QWCursorShapeManagerV1Private(wlr_cursor_shape_manager_v1 *handle, bool isOwner, QWCursorShapeManagerV1 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.request_set_shape, this, &QWCursorShapeManagerV1Private::on_request_set_shape);
    }

    void on_request_set_shape(void *);

    QW_DECLARE_PUBLIC(QWCursorShapeManagerV1)
};

void QWCursorShapeManagerV1Private::on_request_set_shape(void *data)
{
    Q_EMIT q_func()->requestSetShape(static_cast<wlr_cursor_shape_manager_v1_request_set_shape_event*>(data));
}

QWCursorShapeManagerV1::QWCursorShapeManagerV1(wlr_cursor_shape_manager_v1 *handle, bool isOwner)
    : QWWrapObject(*new QWCursorShapeManagerV1Private(handle, isOwner, this))
{

}

QWCursorShapeManagerV1 *QWCursorShapeManagerV1::get(wlr_cursor_shape_manager_v1 *handle)
{
    return static_cast<QWCursorShapeManagerV1*>(QWCursorShapeManagerV1Private::map.value(handle));
}

QWCursorShapeManagerV1 *QWCursorShapeManagerV1::from(wlr_cursor_shape_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWCursorShapeManagerV1(handle, false);
}

QWCursorShapeManagerV1 *QWCursorShapeManagerV1::create(QWDisplay *display, uint32_t version)
{
    auto *handle = wlr_cursor_shape_manager_v1_create(display->handle(), version);
    if (!handle)
        return nullptr;
    return new QWCursorShapeManagerV1(handle, true);
}

QW_END_NAMESPACE
