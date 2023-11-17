// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwdatadevice.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_data_device.h>
}

QW_BEGIN_NAMESPACE

class QWDragPrivate : public QWObjectPrivate
{
public:
    QWDragPrivate(wlr_drag *handle, bool isOwner, QWDrag *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.focus, this, &QWDragPrivate::on_focus);
        sc.connect(&handle->events.motion, this, &QWDragPrivate::on_motion);
        sc.connect(&handle->events.drop, this, &QWDragPrivate::on_drop);
        sc.connect(&handle->events.destroy, this, &QWDragPrivate::on_destroy);
    }

    ~QWDragPrivate() {
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

    void on_focus(void *);
    void on_motion(void *);
    void on_drop(void *);
    void on_destroy(void *);

    static QHash<void*, QWDrag*> map;
    QW_DECLARE_PUBLIC(QWDrag)
    QWSignalConnector sc;
};
QHash<void*, QWDrag*> QWDragPrivate::map;

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

void QWDragPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWDrag::QWDrag(wlr_drag *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWDragPrivate(handle, isOwner, this))
{

}

QWDrag *QWDrag::get(wlr_drag *handle)
{
    return QWDragPrivate::map.value(handle);
}

QWDrag *QWDrag::from(wlr_drag *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWDrag(handle, false);
}

QW_END_NAMESPACE
