// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwinputdevice.h>
#include <QObject>

struct wlr_tablet;
struct wlr_tablet_tool;

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

class QWTabletToolPrivate;
class QW_EXPORT QWTabletTool : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTabletTool)
public:
    inline wlr_tablet_tool *handle() const {
        return QWObject::handle<wlr_tablet_tool>();
    }

    static QWTabletTool *get(wlr_tablet_tool *handle);
    static QWTabletTool *from(wlr_tablet_tool *handle);

Q_SIGNALS:
    void beforeDestroy(QWTabletTool *self);

private:
    QWTabletTool(wlr_tablet_tool *handle, bool isOwner);
    ~QWTabletTool() = default;
};

QW_END_NAMESPACE
