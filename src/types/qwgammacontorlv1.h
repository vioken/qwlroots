// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qw_global.h>
#include <QObject>

struct wlr_output_state;
struct wlr_gamma_control_v1;
struct wlr_gamma_control_manager_v1;
struct wlr_gamma_control_manager_v1_set_gamma_event;

QW_BEGIN_NAMESPACE

class QWOutput;
class QWDisplay;
class QWGammaControlManagerV1Private;

class QW_EXPORT QWGammaControl
{
public:
    QWGammaControl() = delete;
    ~QWGammaControl() = delete;

    wlr_gamma_control_v1 *handle() const;

    static QWGammaControl *from(wlr_gamma_control_v1 *handle);

    void sendFailedAndDestroy();
    bool gammaControlApply(struct wlr_output_state *output_state);
};

class QW_EXPORT QWGammaControlManagerV1 : public QWWrapObject
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

    QWGammaControl *getControl(QWOutput *output);

Q_SIGNALS:
    void gammaChanged(wlr_gamma_control_manager_v1_set_gamma_event *event);

private:
    QWGammaControlManagerV1(wlr_gamma_control_manager_v1 *handle, bool isOwner);
    ~QWGammaControlManagerV1() = default;
};

QW_END_NAMESPACE
