// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <QObject>

struct wlr_tablet_pad;
struct wlr_tablet_tool;

QW_BEGIN_NAMESPACE

class QWTabletPadPrivate;

class QW_EXPORT QWTabletPad : public QWInputDevice
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletPad)
public:
    inline wlr_tablet_pad *handle() const {
        return QWObject::handle<wlr_tablet_pad>();
    }

    static QWTabletPad *get(wlr_tablet_pad *handle);
    static QWTabletPad *from(wlr_tablet_pad *handle);
    static QWTabletPad *fromInputDevice(wlr_input_device *input_device);

Q_SIGNALS:
    void button();
    void ring();
    void strip();
    void attachTablet(wlr_tablet_tool *table);

private:
    ~QWTabletPad() override = default;
    QWTabletPad(wlr_tablet_pad *handle, bool isOwner);
};

QW_END_NAMESPACE
