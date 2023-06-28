// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_input_inhibit_manager;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWInputInhibitManagerPrivate;
class QW_EXPORT QWInputInhibitManager : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWInputInhibitManager)
public:
    inline wlr_input_inhibit_manager *handle() const {
        return QWObject::handle<wlr_input_inhibit_manager>();
    }

    static QWInputInhibitManager *get(wlr_input_inhibit_manager *handle);
    static QWInputInhibitManager *from(wlr_input_inhibit_manager *handle);
    static QWInputInhibitManager *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWInputInhibitManager *self);
    void activate();
    void deactivate();

private:
    QWInputInhibitManager(wlr_input_inhibit_manager *handle, bool isOwner);
    ~QWInputInhibitManager() = default;
};

QW_END_NAMESPACE

