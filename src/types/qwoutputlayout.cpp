// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputlayout.h"
#include "util/qwsignalconnector.h"
#include "qwoutput.h"

#include <QPointF>
#include <QRect>
#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_layout.h>
#undef static
#include <wlr/util/box.h>
}

static_assert(std::is_same_v<wlr_direction_t, std::underlying_type_t<wlr_direction>>);

QW_BEGIN_NAMESPACE

class QWOutputLayoutPrivate : public QWObjectPrivate
{
public:
    QWOutputLayoutPrivate(wlr_output_layout *handle, bool isOwner, QWOutputLayout *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWOutputLayoutPrivate::on_destroy);
        sc.connect(&handle->events.add, this, &QWOutputLayoutPrivate::on_add);
        sc.connect(&handle->events.change, this, &QWOutputLayoutPrivate::on_change);
    }
    ~QWOutputLayoutPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_output_layout_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_add(void *data);
    void on_change(void *data);

    static QHash<void*, QWOutputLayout*> map;
    QW_DECLARE_PUBLIC(QWOutputLayout)
    QWSignalConnector sc;
};
QHash<void*, QWOutputLayout*> QWOutputLayoutPrivate::map;

void QWOutputLayoutPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWOutputLayoutPrivate::on_add(void *data)
{
    Q_EMIT q_func()->add(reinterpret_cast<wlr_output_layout_output*>(data));
}

void QWOutputLayoutPrivate::on_change(void *data)
{
    Q_EMIT q_func()->change(reinterpret_cast<wlr_output_layout_output*>(data));
}

QWOutputLayout::QWOutputLayout(wlr_output_layout *handle, bool isOwner, QObject *parent)
    : QObject(nullptr)
    , QWObject(*new QWOutputLayoutPrivate(handle, isOwner, this))
{

}

QWOutputLayout::QWOutputLayout(QObject *parent)
    : QWOutputLayout(wlr_output_layout_create(), true, parent)
{

}

QWOutputLayout *QWOutputLayout::get(wlr_output_layout *handle)
{
    return QWOutputLayoutPrivate::map.value(handle);
}

QWOutputLayout *QWOutputLayout::from(wlr_output_layout *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWOutputLayout(handle, false, nullptr);
}

wlr_output_layout_output *QWOutputLayout::get(QWOutput *reference) const
{
    return wlr_output_layout_get(handle(), reference->handle());
}

QWOutput *QWOutputLayout::outputAt(const QPointF &pos) const
{
    return QWOutput::from(wlr_output_layout_output_at(handle(), pos.x(), pos.y()));
}

QWOutput *QWOutputLayout::getCenterOutput() const
{
    return QWOutput::from(wlr_output_layout_get_center_output(handle()));
}

QWOutput *QWOutputLayout::adjacentOutput(wlr_direction_t direction, QWOutput *reference, const QPoint &pos) const
{
    return QWOutput::from(wlr_output_layout_adjacent_output(handle(), static_cast<wlr_direction>(direction),
                                             reference->handle(), pos.x(), pos.y()));
}

QWOutput *QWOutputLayout::farthestOutput(wlr_direction_t direction, QWOutput *reference, const QPoint &pos) const
{
    return QWOutput::from(wlr_output_layout_farthest_output(handle(), static_cast<wlr_direction>(direction),
                                             reference->handle(), pos.x(), pos.y()));
}

void QWOutputLayout::add(QWOutput *output, const QPoint &pos)
{
    wlr_output_layout_add(handle(), output->handle(), pos.x(), pos.y());
}

void QWOutputLayout::addAuto(QWOutput *output)
{
    wlr_output_layout_add_auto(handle(), output->handle());
}

void QWOutputLayout::move(QWOutput *output, const QPoint &pos)
{
#if WLR_VERSION_MINOR > 16
    wlr_output_layout_add(handle(), output->handle(), pos.x(), pos.y());
#else
    wlr_output_layout_move(handle(), output->handle(), pos.x(), pos.y());
#endif
}

void QWOutputLayout::remove(QWOutput *output)
{
    wlr_output_layout_remove(handle(), output->handle());
}

QPointF QWOutputLayout::outputCoords(QWOutput *reference) const
{
    QPointF pos;
    wlr_output_layout_output_coords(handle(), reference->handle(), &pos.rx(), &pos.ry());
    return pos;
}

bool QWOutputLayout::containsPoint(QWOutput *reference, const QPoint &pos) const
{
    return wlr_output_layout_contains_point(handle(), reference->handle(), pos.x(), pos.y());
}

bool QWOutputLayout::intersects(QWOutput *reference, const QRect &targetBox) const
{
    wlr_box box {
        .x = targetBox.x(),
        .y = targetBox.y(),
        .width = targetBox.width(),
        .height = targetBox.height()
    };
    return wlr_output_layout_intersects(handle(), reference->handle(), &box);
}

QPointF QWOutputLayout::closestPoint(QWOutput *reference, const QPointF &pos) const
{
    QPointF rpos;
    wlr_output_layout_closest_point(handle(), reference->handle(), pos.x(), pos.y(), &rpos.rx(), &rpos.ry());
    return rpos;
}

QRect QWOutputLayout::getBox(QWOutput *reference) const
{
    wlr_box box;
    wlr_output_layout_get_box(handle(), reference->handle(), &box);
    return QRect(box.x, box.y, box.width, box.height);
}

QW_END_NAMESPACE
