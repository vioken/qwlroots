// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwoutputlayout.h"
#include "private/qwglobal_p.h"
#include "qwoutput.h"
#include "qwdisplay.h"

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

class QWOutputLayoutPrivate : public QWWrapObjectPrivate
{
public:
    QWOutputLayoutPrivate(wlr_output_layout *handle, bool isOwner, QWOutputLayout *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy,
                              toDestroyFunction(wlr_output_layout_destroy))
    {
        sc.connect(&handle->events.add, this, &QWOutputLayoutPrivate::on_add);
        sc.connect(&handle->events.change, this, &QWOutputLayoutPrivate::on_change);
    }

    void on_add(void *data);
    void on_change(void *data);

    QW_DECLARE_PUBLIC(QWOutputLayout)
};

void QWOutputLayoutPrivate::on_add(void *data)
{
    Q_EMIT q_func()->add(reinterpret_cast<wlr_output_layout_output*>(data));
}

void QWOutputLayoutPrivate::on_change(void *data)
{
    Q_EMIT q_func()->change(reinterpret_cast<wlr_output_layout_output*>(data));
}

QWOutputLayout::QWOutputLayout(wlr_output_layout *handle, bool isOwner, QObject *parent)
    : QWWrapObject(*new QWOutputLayoutPrivate(handle, isOwner, this), parent)
{

}

#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
QWOutputLayout::QWOutputLayout(QObject *parent)
    : QWOutputLayout(wlr_output_layout_create(), true, parent)
{

}
#else
QWOutputLayout::QWOutputLayout(QWDisplay *display, QObject *parent)
    : QWOutputLayout(wlr_output_layout_create(display->handle()), true, parent)
{

}
#endif

QWOutputLayout *QWOutputLayout::get(wlr_output_layout *handle)
{
    return static_cast<QWOutputLayout*>(QWOutputLayoutPrivate::map.value(handle));
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
    wlr_output_layout_add(handle(), output->handle(), pos.x(), pos.y());
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
