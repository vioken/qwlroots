// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_gamma_control_manager_v1;
struct wlr_gamma_control_manager_v1_set_gamma_event;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWGammaControlManagerV1Private;
class QW_EXPORT QWGammaControlManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWGammaControlManagerV1)
public:
    inline wlr_gamma_control_manager_v1 *handle() const {
        return QWObject::handle<wlr_gamma_control_manager_v1>();
    }

    static QWGammaControlManagerV1 *get(wlr_gamma_control_manager_v1 *handle);
    static QWGammaControlManagerV1 *from(wlr_gamma_control_manager_v1 *handle);
    static QWGammaControlManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void beforeDestroy(QWGammaControlManagerV1 *self);
    void gammaChanged(wlr_gamma_control_manager_v1_set_gamma_event *event);

private:
    QWGammaControlManagerV1(wlr_gamma_control_manager_v1 *handle, bool isOwner);
    ~QWGammaControlManagerV1() = default;
};

QW_END_NAMESPACE
