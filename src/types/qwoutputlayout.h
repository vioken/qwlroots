// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qw_global.h>
#include <QObject>

struct wlr_output_layout;
struct wlr_output_layout_output;

typedef uint32_t wlr_direction_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWOutput;
class QWRenderer;
class QWOutputLayoutPrivate;
class QW_EXPORT QWOutputLayout : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWOutputLayout)
public:
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
    explicit QWOutputLayout(QObject *parent = nullptr);
#else
    explicit QWOutputLayout(QWDisplay *display, QObject *parent = nullptr);
#endif
    ~QWOutputLayout() = default;

    inline wlr_output_layout *handle() const {
        return QWObject::handle<wlr_output_layout>();
    }

    static QWOutputLayout *get(wlr_output_layout *handle);
    static QWOutputLayout *from(wlr_output_layout *handle);

    wlr_output_layout_output *get(QWOutput *reference) const;
    QWOutput *outputAt(const QPointF &pos) const;
    QWOutput *getCenterOutput() const;
    QWOutput *adjacentOutput(wlr_direction_t wlr_direction, QWOutput *reference, const QPoint &pos) const;
    QWOutput *farthestOutput(wlr_direction_t wlr_direction, QWOutput *reference, const QPoint &pos) const;

    void add(QWOutput *output, const QPoint &pos);
    void addAuto(QWOutput *output);
    void move(QWOutput *output, const QPoint &pos);
    void remove(QWOutput *output);

    QPointF outputCoords(QWOutput *reference) const;
    bool containsPoint(QWOutput *reference, const QPoint &pos) const;
    bool intersects(QWOutput *reference, const QRect &targetBox) const;
    QPointF closestPoint(QWOutput *reference, const QPointF &pos) const;
    QRect getBox(QWOutput *reference) const;

Q_SIGNALS:
    // TODO: make to QWOutputLayoutOutput
    void add(wlr_output_layout_output *output);
    void change(wlr_output_layout_output *output);

private:
    QWOutputLayout(wlr_output_layout *handle, bool isOwner, QObject *parent);
};

QW_END_NAMESPACE
