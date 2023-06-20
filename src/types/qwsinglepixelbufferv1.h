// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_single_pixel_buffer_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWSinglePixelBufferManagerV1Private;
class QW_EXPORT QWSinglePixelBufferManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWSinglePixelBufferManagerV1)
public:
    static QWSinglePixelBufferManagerV1 *get(wlr_single_pixel_buffer_manager_v1 *handle);
    static QWSinglePixelBufferManagerV1 *from(wlr_single_pixel_buffer_manager_v1 *handle);
    static QWSinglePixelBufferManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWSinglePixelBufferManagerV1 *self);

private:
    QWSinglePixelBufferManagerV1(wlr_single_pixel_buffer_manager_v1 *handle, bool isOwner);
};

QW_END_NAMESPACE

