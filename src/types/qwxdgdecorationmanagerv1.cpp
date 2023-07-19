// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxdgdecorationmanagerv1.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_decoration_v1.h>
#undef static
}

static_assert(std::is_same_v<wlr_xdg_toplevel_decoration_v1_mode_t, std::underlying_type_t<wlr_xdg_toplevel_decoration_v1_mode>>);

QW_BEGIN_NAMESPACE

/// QWXdgDecorationManagerV1

class QWXdgDecorationManagerV1Private : public QWObjectPrivate
{
public:
    QWXdgDecorationManagerV1Private(wlr_xdg_decoration_manager_v1 *handle, bool isOwner, QWXdgDecorationManagerV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgDecorationManagerV1Private::on_destroy);
        sc.connect(&handle->events.new_toplevel_decoration, this, &QWXdgDecorationManagerV1Private::on_new_toplevel_decoration);
    }
    ~QWXdgDecorationManagerV1Private() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_new_toplevel_decoration(void *);

    static QHash<void*, QWXdgDecorationManagerV1*> map;
    QW_DECLARE_PUBLIC(QWXdgDecorationManagerV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgDecorationManagerV1*> QWXdgDecorationManagerV1Private::map;

void QWXdgDecorationManagerV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWXdgDecorationManagerV1Private::on_new_toplevel_decoration(void *data)
{
    auto *decoration = QWXdgToplevelDecorationV1::from(reinterpret_cast<wlr_xdg_toplevel_decoration_v1*>(data));
    Q_EMIT q_func()->newToplevelDecoration(decoration);
}

QWXdgDecorationManagerV1::QWXdgDecorationManagerV1(wlr_xdg_decoration_manager_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgDecorationManagerV1Private(handle, isOwner, this))
{

}

QWXdgDecorationManagerV1 *QWXdgDecorationManagerV1::get(wlr_xdg_decoration_manager_v1 *handle)
{
    return QWXdgDecorationManagerV1Private::map.value(handle);
}

QWXdgDecorationManagerV1 *QWXdgDecorationManagerV1::from(wlr_xdg_decoration_manager_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgDecorationManagerV1(handle, false);
}

QWXdgDecorationManagerV1 *QWXdgDecorationManagerV1::create(QWDisplay *display)
{
    auto *handle = wlr_xdg_decoration_manager_v1_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWXdgDecorationManagerV1(handle, true);
}

/// QWXdgToplevelDecorationV1

class QWXdgToplevelDecorationV1Private : public QWObjectPrivate
{
public:
    QWXdgToplevelDecorationV1Private(wlr_xdg_toplevel_decoration_v1 *handle, bool isOwner, QWXdgToplevelDecorationV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWXdgToplevelDecorationV1Private::on_destroy);
        sc.connect(&handle->events.request_mode, this, &QWXdgToplevelDecorationV1Private::on_request_mode);
    }
    ~QWXdgToplevelDecorationV1Private() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_request_mode(void *);

    static QHash<void*, QWXdgToplevelDecorationV1*> map;
    QW_DECLARE_PUBLIC(QWXdgToplevelDecorationV1)
    QWSignalConnector sc;
};
QHash<void*, QWXdgToplevelDecorationV1*> QWXdgToplevelDecorationV1Private::map;

void QWXdgToplevelDecorationV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWXdgToplevelDecorationV1Private::on_request_mode(void *)
{
    Q_EMIT q_func()->requestMode();
}

QWXdgToplevelDecorationV1::QWXdgToplevelDecorationV1(wlr_xdg_toplevel_decoration_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWXdgToplevelDecorationV1Private(handle, isOwner, this))
{

}

QWXdgToplevelDecorationV1 *QWXdgToplevelDecorationV1::get(wlr_xdg_toplevel_decoration_v1 *handle)
{
    return QWXdgToplevelDecorationV1Private::map.value(handle);
}

QWXdgToplevelDecorationV1 *QWXdgToplevelDecorationV1::from(wlr_xdg_toplevel_decoration_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWXdgToplevelDecorationV1(handle, false);
}

void QWXdgToplevelDecorationV1::setMode(wlr_xdg_toplevel_decoration_v1_mode_t mode)
{
    wlr_xdg_toplevel_decoration_v1_set_mode(handle(), static_cast<wlr_xdg_toplevel_decoration_v1_mode>(mode));
}

QW_END_NAMESPACE
