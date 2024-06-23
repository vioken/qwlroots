// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qw_global.h>
#include <QObject>

struct wlr_output_layout;
struct wlr_xdg_output_manager_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWOutputLayout;
class QWXdgOutputManagerV1Private;
class QW_EXPORT QWXdgOutputManagerV1 : public QWWrapObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgOutputManagerV1)
public:
    inline wlr_xdg_output_manager_v1 *handle() const {
        return QWObject::handle<wlr_xdg_output_manager_v1>();
    }

    static QWXdgOutputManagerV1 *get(wlr_xdg_output_manager_v1 *handle);
    static QWXdgOutputManagerV1 *from(wlr_xdg_output_manager_v1 *handle);
    static QWXdgOutputManagerV1 *create(QWDisplay *display, QWOutputLayout *layout);

private:
    QWXdgOutputManagerV1(wlr_xdg_output_manager_v1 *handle, bool isOwner);
    ~QWXdgOutputManagerV1() override = default;
};

QW_END_NAMESPACE

