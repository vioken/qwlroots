// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_primary_selection_v1_device_manager;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWPrimarySelectionV1DeviceManagerPrivate;
class QW_EXPORT QWPrimarySelectionV1DeviceManager : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPrimarySelectionV1DeviceManager)
public:
    inline wlr_primary_selection_v1_device_manager *handle() const {
        return QWObject::handle<wlr_primary_selection_v1_device_manager>();
    }

    static QWPrimarySelectionV1DeviceManager *get(wlr_primary_selection_v1_device_manager *handle);
    static QWPrimarySelectionV1DeviceManager *from(wlr_primary_selection_v1_device_manager *handle);
    static QWPrimarySelectionV1DeviceManager *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWPrimarySelectionV1DeviceManager *self);

private:
    QWPrimarySelectionV1DeviceManager(wlr_primary_selection_v1_device_manager *handle, bool isOwner);
    ~QWPrimarySelectionV1DeviceManager() = default;
};

QW_END_NAMESPACE

