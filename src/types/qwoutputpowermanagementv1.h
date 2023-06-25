// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_output_power_manager_v1;
struct wlr_output_power_v1_set_mode_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWOutputPowerManagerV1Private;
class QW_EXPORT QWOutputPowerManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWOutputPowerManagerV1)
public:
    inline wlr_output_power_manager_v1 *handle() const {
        return QWObject::handle<wlr_output_power_manager_v1>();
    }

    static QWOutputPowerManagerV1 *get(wlr_output_power_manager_v1 *handle);
    static QWOutputPowerManagerV1 *from(wlr_output_power_manager_v1 *handle);
    static QWOutputPowerManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWOutputPowerManagerV1 *self);
    void modeChanged(wlr_output_power_v1_set_mode_event* event);

private:
    QWOutputPowerManagerV1(wlr_output_power_manager_v1 *handle, bool isOwner);
    ~QWOutputPowerManagerV1() = default;
};

QW_END_NAMESPACE

