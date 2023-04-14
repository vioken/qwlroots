// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wl_display;

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

QW_BEGIN_NAMESPACE

class QWDisplayPrivate;
class QW_EXPORT QWDisplay : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWDisplay)
public:
    explicit QWDisplay(QObject *parent = nullptr);

    inline wl_display *handle() const {
        return QWObject::handle<wl_display>();
    }

    int addSocket(const char *name);
    int addSocketFd(int fd);
    const char *addSocketAuto();

    void exec();
    void start(QThread *thread);

Q_SIGNALS:
    void beforeDestroy(QWDisplay *self);

public Q_SLOTS:
    void terminate();
};

QW_END_NAMESPACE
