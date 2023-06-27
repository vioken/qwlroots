// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_subcompositor;
struct wlr_subsurface;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSubcompositorPrivate;
class QW_EXPORT QWSubcompositor : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSubcompositor)
public:
    inline wlr_subcompositor *handle() const {
        return QWObject::handle<wlr_subcompositor>();
    }

    static QWSubcompositor *get(wlr_subcompositor *handle);
    static QWSubcompositor *from(wlr_subcompositor *handle);
    static QWSubcompositor *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWSubcompositor *self);

private:
    QWSubcompositor(wlr_subcompositor *handle, bool isOwner);
    ~QWSubcompositor() = default;
};

QW_END_NAMESPACE
