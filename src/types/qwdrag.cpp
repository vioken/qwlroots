// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

QW_BEGIN_NAMESPACE

class QWDragPrivate : public QWWrapObjectPrivate
{
public:
    QWDragPrivate(wlr_drag *handle, bool isOwner, QWDrag *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.focus, this, &QWDragPrivate::on_focus);
        sc.connect(&handle->events.motion, this, &QWDragPrivate::on_motion);
        sc.connect(&handle->events.drop, this, &QWDragPrivate::on_drop);
    }

    void on_focus(void *);
    void on_motion(void *);
    void on_drop(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWDrag)
};
QHash<void*, QWWrapObject*> QWDragPrivate::map;

void QWDragPrivate::on_focus(void *)
{
    Q_EMIT q_func()->focus();
}

void QWDragPrivate::on_motion(void *data)
{
    Q_EMIT q_func()->motion(reinterpret_cast<wlr_drag_motion_event*>(data));
}

void QWDragPrivate::on_drop(void *data)
{
    Q_EMIT q_func()->drop(reinterpret_cast<wlr_drag_drop_event*>(data));
}

QWDrag::QWDrag(wlr_drag *handle, bool isOwner)
    : QWWrapObject(*new QWDragPrivate(handle, isOwner, this))
{

}

QWDrag *QWDrag::get(wlr_drag *handle)
{
    return static_cast<QWDrag*>(QWDragPrivate::map.value(handle));
}

QWDrag *QWDrag::from(wlr_drag *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDrag(handle, false);
}

QW_END_NAMESPACE
