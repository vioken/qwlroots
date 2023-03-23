// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

// WARNING: This file is not part of the QWlroots API.

#include "qwglobal.h"
#include "util/qwsignalconnector.h"

#include <QObject>
#include <QHash>

struct wlr_input_device;

QW_BEGIN_NAMESPACE

class QWInputDevice;

class QWInputDevicePrivate : public QWObjectPrivate
{
public:
    QWInputDevicePrivate(wlr_input_device *handle, bool isOwner, QWInputDevice *qq);
    virtual ~QWInputDevicePrivate() override;

    inline void destroy();
    void on_destroy(void *);

    static QHash<void*, QWInputDevice*> map;
    QW_DECLARE_PUBLIC(QWInputDevice)
    QWSignalConnector sc;
};

QW_END_NAMESPACE
