// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_xdg_decoration_manager_v1;
struct wlr_xdg_toplevel_decoration_v1;

typedef uint32_t wlr_xdg_toplevel_decoration_v1_mode_t;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXdgToplevelDecorationV1;
class QWXdgDecorationManagerV1Private;
class QW_EXPORT QWXdgDecorationManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgDecorationManagerV1)
public:
    inline wlr_xdg_decoration_manager_v1 *handle() const {
        return QWObject::handle<wlr_xdg_decoration_manager_v1>();
    }

    static QWXdgDecorationManagerV1 *get(wlr_xdg_decoration_manager_v1 *handle);
    static QWXdgDecorationManagerV1 *from(wlr_xdg_decoration_manager_v1 *handle);
    static QWXdgDecorationManagerV1 *create(QWDisplay *display);

Q_SIGNALS:
    void newToplevelDecoration(QWXdgToplevelDecorationV1 *decorat);

private:
    QWXdgDecorationManagerV1(wlr_xdg_decoration_manager_v1 *handle, bool isOwner);
    ~QWXdgDecorationManagerV1() = default;
};

class QWXdgToplevelDecorationV1Private;
class QW_EXPORT QWXdgToplevelDecorationV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgToplevelDecorationV1)
public:
    inline wlr_xdg_toplevel_decoration_v1 *handle() const {
        return QWObject::handle<wlr_xdg_toplevel_decoration_v1>();
    }

    static QWXdgToplevelDecorationV1 *get(wlr_xdg_toplevel_decoration_v1 *handle);
    static QWXdgToplevelDecorationV1 *from(wlr_xdg_toplevel_decoration_v1 *handle);

    void setMode(wlr_xdg_toplevel_decoration_v1_mode_t mode);

Q_SIGNALS:
    void requestMode();

private:
    QWXdgToplevelDecorationV1(wlr_xdg_toplevel_decoration_v1 *handle, bool isOwner);
    ~QWXdgToplevelDecorationV1() = default;
};

QW_END_NAMESPACE
