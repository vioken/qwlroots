// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;

QW_BEGIN_NAMESPACE

class QWBackendPrivate;
class QW_EXPORT QWBackend : public QObject, public QWObject
{
    QW_DECLARE_PRIVATE(QWBackend)
public:
    static QWBackend *autoCreate(wl_display *display, QObject *parent = nullptr);
    ~QWBackend();

    clockid_t presentationClock() const;
    int drmFd() const;

public Q_SLOTS:
    bool start();

private:
    explicit QWBackend(void *handle, QObject *parent = nullptr);
};

QW_END_NAMESPACE
