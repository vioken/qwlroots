// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_input_device;

QW_BEGIN_NAMESPACE

class QWInputDevicePrivate;
class QW_EXPORT QWInputDevice : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWInputDevice)
public:
    inline wlr_input_device *handle() const {
        return QWObject::handle<wlr_input_device>();
    }

    static QWInputDevice *get(wlr_input_device *handle);
    static QWInputDevice *from(wlr_input_device *handle);

Q_SIGNALS:
    void beforeDestroy(QWInputDevice *self);

protected:
    QWInputDevice(QWObjectPrivate &dd);
    virtual ~QWInputDevice() override = default;
private:
    QWInputDevice(wlr_input_device *handle, bool isOwner);
};

QW_END_NAMESPACE
