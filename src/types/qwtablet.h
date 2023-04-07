// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <QObject>

struct wlr_tablet;

QW_BEGIN_NAMESPACE

class QWTabletPrivate;

class QW_EXPORT QWTablet : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTablet)
public:
    inline wlr_tablet *handle() const {
        return QWObject::handle<wlr_tablet>();
    }

    static QWTablet *get(wlr_tablet *handle);
    static QWTablet *from(wlr_tablet *handle);
    static QWTablet *fromInputDevice(wlr_input_device *input_device);

Q_SIGNALS:
    void axis();
    void proximity();
    void tip();
    void button();

private:
    ~QWTablet() override = default;
    QWTablet(wlr_tablet *handle, bool isOwner);
};

QW_END_NAMESPACE
