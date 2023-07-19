// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwforeigntoplevelhandlev1.h"
#include "qwoutput.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include "math.h"
#define static
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>
#undef static
}

QW_BEGIN_NAMESPACE

class QWForeignToplevelHandleV1Private : public QWObjectPrivate
{
public:
    QWForeignToplevelHandleV1Private(wlr_foreign_toplevel_handle_v1 *handle, bool isOwner, QWForeignToplevelHandleV1 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWForeignToplevelHandleV1Private::on_destroy);
        sc.connect(&handle->events.request_maximize, this, &QWForeignToplevelHandleV1Private::on_request_maximize);
        sc.connect(&handle->events.request_minimize, this, &QWForeignToplevelHandleV1Private::on_request_minimize);
        sc.connect(&handle->events.request_activate, this, &QWForeignToplevelHandleV1Private::on_request_activate);
        sc.connect(&handle->events.request_fullscreen, this, &QWForeignToplevelHandleV1Private::on_request_fullscreen);
        sc.connect(&handle->events.request_close, this, &QWForeignToplevelHandleV1Private::on_request_close);
        sc.connect(&handle->events.set_rectangle, this, &QWForeignToplevelHandleV1Private::on_set_rectangle);
    }
    ~QWForeignToplevelHandleV1Private() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_foreign_toplevel_handle_v1_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);
    void on_request_maximize(void *);
    void on_request_minimize(void *);
    void on_request_activate(void *);
    void on_request_fullscreen(void *);
    void on_request_close(void *);
    void on_set_rectangle(void *);

    static QHash<void*, QWForeignToplevelHandleV1*> map;
    QW_DECLARE_PUBLIC(QWForeignToplevelHandleV1)
    QWSignalConnector sc;
};
QHash<void*, QWForeignToplevelHandleV1*> QWForeignToplevelHandleV1Private::map;

void QWForeignToplevelHandleV1Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWForeignToplevelHandleV1Private::on_request_maximize(void *data)
{
    Q_EMIT q_func()->requestMaximize(reinterpret_cast<wlr_foreign_toplevel_handle_v1_maximized_event*>(data));
}

void QWForeignToplevelHandleV1Private::on_request_minimize(void *data)
{
    Q_EMIT q_func()->requestMinimize(reinterpret_cast<wlr_foreign_toplevel_handle_v1_minimized_event*>(data));
}

void QWForeignToplevelHandleV1Private::on_request_activate(void *data)
{
    Q_EMIT q_func()->requestActivate(reinterpret_cast<wlr_foreign_toplevel_handle_v1_activated_event*>(data));
}

void QWForeignToplevelHandleV1Private::on_request_fullscreen(void *data)
{
    Q_EMIT q_func()->requestFullscreen(reinterpret_cast<wlr_foreign_toplevel_handle_v1_fullscreen_event*>(data));
}

void QWForeignToplevelHandleV1Private::on_request_close(void *)
{
    Q_EMIT q_func()->requestClose();
}

void QWForeignToplevelHandleV1Private::on_set_rectangle(void *data)
{
    Q_EMIT q_func()->rectangleChanged(reinterpret_cast<wlr_foreign_toplevel_handle_v1_set_rectangle_event*>(data));
}

QWForeignToplevelHandleV1::QWForeignToplevelHandleV1(wlr_foreign_toplevel_handle_v1 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWForeignToplevelHandleV1Private(handle, isOwner, this))
{

}

QWForeignToplevelHandleV1 *QWForeignToplevelHandleV1::get(wlr_foreign_toplevel_handle_v1 *handle)
{
    return QWForeignToplevelHandleV1Private::map.value(handle);
}

QWForeignToplevelHandleV1 *QWForeignToplevelHandleV1::from(wlr_foreign_toplevel_handle_v1 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWForeignToplevelHandleV1(handle, false);
}

QWForeignToplevelHandleV1 *QWForeignToplevelHandleV1::create(QWForeignToplevelManagerV1 *manager)
{
    auto *handle = wlr_foreign_toplevel_handle_v1_create(manager->handle());
    if (!handle)
        return nullptr;
    return new QWForeignToplevelHandleV1(handle, true);
}

void QWForeignToplevelHandleV1::outputEnter(QWOutput *output)
{
    wlr_foreign_toplevel_handle_v1_output_enter(handle(), output->handle());
}

void QWForeignToplevelHandleV1::outputLeave(QWOutput *output)
{
    wlr_foreign_toplevel_handle_v1_output_leave(handle(), output->handle());
}

void QWForeignToplevelHandleV1::setActivated(bool activated)
{
    wlr_foreign_toplevel_handle_v1_set_activated(handle(), activated);
}

void QWForeignToplevelHandleV1::setAppId(const char *appId)
{
    wlr_foreign_toplevel_handle_v1_set_app_id(handle(), appId);
}

void QWForeignToplevelHandleV1::setFullScreen(bool fullScreen)
{
    wlr_foreign_toplevel_handle_v1_set_fullscreen(handle(), fullScreen);
}

void QWForeignToplevelHandleV1::setMaximized(bool maximized)
{
    wlr_foreign_toplevel_handle_v1_set_maximized(handle(), maximized);
}

void QWForeignToplevelHandleV1::setMinimized(bool minimized)
{
    wlr_foreign_toplevel_handle_v1_set_minimized(handle(), minimized);
}

void QWForeignToplevelHandleV1::setParent(QWForeignToplevelHandleV1 *parent)
{
    wlr_foreign_toplevel_handle_v1_set_parent(handle(), parent->handle());
}

void QWForeignToplevelHandleV1::setTitle(const char *title)
{
    wlr_foreign_toplevel_handle_v1_set_title(handle(), title);
}

QW_END_NAMESPACE
