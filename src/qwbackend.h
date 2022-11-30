// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_backend;
struct wlr_input_device;
struct wlr_output;
struct wl_display;

QW_BEGIN_NAMESPACE

class QWBackendPrivate;
class QW_EXPORT QWBackend : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWBackend)
public:
    explicit QWBackend(wlr_backend *handle, QObject *parent = nullptr);
    ~QWBackend();

    static QWBackend *autoCreate(wl_display *display, QObject *parent = nullptr);

    inline wlr_backend *handle() const {
        return QWObject::handle<wlr_backend>();
    }

    clockid_t presentationClock() const;
    int drmFd() const;

public Q_SLOTS:
    bool start();

Q_SIGNALS:
    // TODO: make to QWInputDevice
    void newInput(wlr_input_device *device);
    // TODO: make to QWOutput
    void newOutput(wlr_output *output);
};

QW_END_NAMESPACE
