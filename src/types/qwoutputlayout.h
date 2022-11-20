// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_output_layout;
struct wlr_output_layout_output;
struct wlr_output;

QW_BEGIN_NAMESPACE

class QWRenderer;
class QWOutputLayoutPrivate;
class QW_EXPORT QWOutputLayout : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWOutputLayout)
public:
    explicit QWOutputLayout(wlr_output_layout *handle);

    static QWOutputLayout *create();

    inline wlr_output_layout *handle() const {
        return QWObject::handle<wlr_output_layout>();
    }

    wlr_output_layout_output *get(wlr_output *reference) const;
    wlr_output *outputAt(const QPointF &pos) const;
    wlr_output *getCenterOutput() const;
    wlr_output *adjacentOutput(int wlr_direction, wlr_output *reference, const QPoint &pos) const;
    wlr_output *farthestOutput(int wlr_direction, wlr_output *reference, const QPoint &pos) const;

    void add(wlr_output *output, const QPoint &pos);
    void addAuto(wlr_output *output);
    void move(wlr_output *output, const QPoint &pos);
    void remove(wlr_output *output);

    QPointF outputCoords(wlr_output *reference) const;
    bool containsPoint(wlr_output *reference, const QPoint &pos) const;
    bool intersects(wlr_output *reference, const QRect &targetBox) const;
    QPointF closestPoint(wlr_output *reference, const QPointF &pos) const;
    QRect getBox(wlr_output *reference) const;


Q_SIGNALS:
    // TODO: make to QWOutputLayoutOutput
    void add(wlr_output_layout_output *output);
    void change(wlr_output_layout_output *output);
};

QW_END_NAMESPACE
