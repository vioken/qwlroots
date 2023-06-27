// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_viewporter;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWViewPorterPrivate;
class QW_EXPORT QWViewPorter : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWViewPorter)
public:
    inline wlr_viewporter *handle() const {
        return QWObject::handle<wlr_viewporter>();
    }

    static QWViewPorter *get(wlr_viewporter *handle);
    static QWViewPorter *from(wlr_viewporter *handle);
    static QWViewPorter *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWViewPorter *self);

private:
    QWViewPorter(wlr_viewporter *handle, bool isOwner);
    ~QWViewPorter() = default;
};

QW_END_NAMESPACE

