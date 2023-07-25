// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwcursorshapev1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_cursor_shape_v1.h>
}

QW_BEGIN_NAMESPACE

class QWCursorShapeManagerV1Private : public QWObjectPrivate
{
public:
    QWCursorShapeManagerV1Private(wlr_cursor_shape_manager_v1 *handle, bool isOwner, QWCursorShapeManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWCursorShapeManagerV1Private::on_destroy);
        sc.connect(&handle->events.request_set_shape, this, &QWCursorShapeManagerV1Private::on_request_set_shape);
    }
    ~QWCursorShapeManagerV1Private() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_request_set_shape(void *);

    static QHash<void*, QWCursorShapeManagerV1*> map;
    QW_DECLARE_PUBLIC(QWCursorShapeManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWCursorShapeManagerV1*> QWCursorShapeManagerV1Private::map;

void QWCursorShapeManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWCursorShapeManagerV1Private::on_request_set_shape(void *data)
{
    Q_EMIT q_func()->requestSetShape(static_cast<wlr_cursor_shape_manager_v1_request_set_shape_event*>(data));
}

QWCursorShapeManagerV1::QWCursorShapeManagerV1(wlr_cursor_shape_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWCursorShapeManagerV1Private(handle, isOwner, this))
{

}

QWCursorShapeManagerV1 *QWCursorShapeManagerV1::get(wlr_cursor_shape_manager_v1 *handle)
{
    return QWCursorShapeManagerV1Private::map.value(handle);
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
