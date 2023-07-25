// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwbackend.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"
#include "types/qwoutput.h"
#include "types/qwkeyboard.h"

#include <QHash>

extern "C" {
#include <wlr/backend.h>
#include <wlr/backend/multi.h>
#define static
#include <wlr/backend/drm.h>
#undef static
#include <wlr/backend/wayland.h>
#ifdef WLR_HAVE_X11_BACKEND
#include <wlr/backend/x11.h>
#endif
#include <wlr/backend/libinput.h>
#include <wlr/backend/headless.h>
}

QW_BEGIN_NAMESPACE

class QWBackendPrivate : public QWObjectPrivate
{
public:
    QWBackendPrivate(wlr_backend *handle, bool isOwner, QWBackend *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.new_input, this, &QWBackendPrivate::on_new_input);
        sc.connect(&handle->events.new_output, this, &QWBackendPrivate::on_new_output);
        sc.connect(&handle->events.destroy, this, &QWBackendPrivate::on_destroy);
    }
    ~QWBackendPrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_backend_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_new_input(wlr_input_device *device);
    void on_new_output(wlr_output *output);
    void on_destroy(void *);

    static QWBackend *createBackend(wlr_backend *handle, bool isOwner, QObject *parent);

    static QHash<void*, QWBackend*> map;
    QW_DECLARE_PUBLIC(QWBackend)
    QWSignalConnector sc;
};
QHash<void*, QWBackend*> QWBackendPrivate::map;

void QWBackendPrivate::on_new_input(wlr_input_device *device)
{
    Q_Q(QWBackend);
    Q_ASSERT(device);
    Q_EMIT q->newInput(QWInputDevice::from(device));
}

void QWBackendPrivate::on_new_output(wlr_output *output)
{
    Q_Q(QWBackend);
    Q_ASSERT(output);
    Q_EMIT q->newOutput(QWOutput::from(output));
}

void QWBackendPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWBackend *QWBackend::get(wlr_backend *handle)
{
    return QWBackendPrivate::map.value(handle);
}

QWBackend *QWBackendPrivate::createBackend(wlr_backend *handle, bool isOwner, QObject *parent)
{
    if (wlr_backend_is_multi(handle))
        return new QWMultiBackend(handle, isOwner, parent);
    #ifdef WLR_HAVE_X11_BACKEND
    if (wlr_backend_is_x11(handle))
        return new QWX11Backend(handle, isOwner, parent);
    #endif
    if (wlr_backend_is_drm(handle))
        return new QWDrmBackend(handle, isOwner, parent);
    if (wlr_backend_is_headless(handle))
        return new QWHeadlessBackend(handle, isOwner, parent);
    if (wlr_backend_is_libinput(handle))
        return new QWLibinputBackend(handle, isOwner, parent);
    if (wlr_backend_is_wl(handle))
        return new QWWaylandBackend(handle, isOwner, parent);

    return new QWBackend(handle, isOwner, parent);
}

QWBackend *QWBackend::from(wlr_backend *handle)
{
    if (auto o = get(handle))
        return o;
    return QWBackendPrivate::createBackend(handle, false, nullptr);
}

QWBackend *QWBackend::autoCreate(QWDisplay *display, QObject *parent)
{
#if WLR_VERSION_MINOR > 16
    auto handle = wlr_backend_autocreate(display->handle(), nullptr);
#else
    auto handle = wlr_backend_autocreate(display->handle());
#endif
    if (!handle)
        return nullptr;
    return QWBackendPrivate::createBackend(handle, true, parent);
}

QWBackend::QWBackend(QWBackendPrivate &dd, QObject *parent)
    : QObject(parent)
    , QWObject(dd)
{

}

QWBackend::QWBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QObject(parent)
    , QWObject(*new QWBackendPrivate(handle, isOwner, this))
{

}

QWBackend::~QWBackend()
{

}

clockid_t QWBackend::presentationClock() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_presentation_clock(handle());
}

int QWBackend::drmFd() const
{
    Q_D(const QWBackend);
    return wlr_backend_get_drm_fd(handle());
}

bool QWBackend::start()
{
    Q_D(QWBackend);
    return wlr_backend_start(handle());
}

QWMultiBackend *QWMultiBackend::get(wlr_backend *handle)
{
    return qobject_cast<QWMultiBackend*>(QWBackend::get(handle));
}

QWMultiBackend *QWMultiBackend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_multi(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWMultiBackend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWMultiBackend(handle, false);
}

QWMultiBackend *QWMultiBackend::create(QWDisplay *display, QObject *parent)
{
    auto handle = wlr_multi_backend_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWMultiBackend(handle, true, parent);
}

bool QWMultiBackend::add(QWBackend *backend)
{
    return wlr_multi_backend_add(handle(), backend->handle());
}

void QWMultiBackend::remove(QWBackend *backend)
{
    return wlr_multi_backend_remove(handle(), backend->handle());
}

bool QWMultiBackend::isEmpty() const
{
    return wlr_multi_is_empty(handle());
}

void QWMultiBackend::forEachBackend(wlr_multi_backend_iterator_func_t iterator, void *userData)
{
    wlr_multi_for_each_backend(handle(), iterator, userData);
}

QWMultiBackend::QWMultiBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

QWDrmBackend *QWDrmBackend::get(wlr_backend *handle)
{
    return qobject_cast<QWDrmBackend*>(QWBackend::get(handle));
}

QWDrmBackend *QWDrmBackend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_drm(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWDrmBackend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWDrmBackend(handle, false);
}

QWDrmBackend *QWDrmBackend::create(QWDisplay *display, wlr_session *session, wlr_device *dev, QWBackend *parent)
{
    auto handle = wlr_drm_backend_create(display->handle(), session, dev, parent->handle());
    if (!handle)
        return nullptr;
    return new QWDrmBackend(handle, true, parent);
}

bool QWDrmBackend::isDrmOutput(QWOutput *output)
{
    return wlr_output_is_drm(output->handle());
}

uint32_t QWDrmBackend::connectorGetId(QWOutput *output)
{
    return wlr_drm_connector_get_id(output->handle());
}

wlr_output_mode *QWDrmBackend::connectorAddMode(QWOutput *output, const drmModeModeInfo *mode)
{
    return wlr_drm_connector_add_mode(output->handle(), mode);
}

wl_output_transform_t QWDrmBackend::connectorGetPanelOrientation(QWOutput *output)
{
    return wlr_drm_connector_get_panel_orientation(output->handle());
}

wlr_drm_lease *QWDrmBackend::createLease(wlr_output **outputs, size_t outputCount, int *leaseFd)
{
    return wlr_drm_create_lease(outputs, outputCount, leaseFd);
}

void QWDrmBackend::terminateLease(wlr_drm_lease *lease)
{
    wlr_drm_lease_terminate(lease);
}

int QWDrmBackend::getNonMasterFd() const
{
    return wlr_drm_backend_get_non_master_fd(handle());
}

QWDrmBackend::QWDrmBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

QWWaylandBackend *QWWaylandBackend::get(wlr_backend *handle)
{
    return qobject_cast<QWWaylandBackend*>(QWBackend::get(handle));
}

QWWaylandBackend *QWWaylandBackend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_wl(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWWaylandBackend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWWaylandBackend(handle, false);
}

QWWaylandBackend *QWWaylandBackend::create(QWDisplay *display, const char *remote, QObject *parent)
{
    auto handle = wlr_wl_backend_create(display->handle(), remote);
    if (!handle)
        return nullptr;
    return new QWWaylandBackend(handle, true, parent);
}

wl_display *QWWaylandBackend::getRemoteDisplay() const
{
    return wlr_wl_backend_get_remote_display(handle());
}

QWOutput *QWWaylandBackend::createOutput()
{
    auto output = wlr_wl_output_create(handle());
    if (!output)
        return nullptr;
    return QWOutput::from(output);
}

bool QWWaylandBackend::isWaylandInputDevice(QWInputDevice *device)
{
    return wlr_input_device_is_wl(device->handle());
}

bool QWWaylandBackend::isWaylandOutput(QWOutput *output)
{
    return wlr_output_is_wl(output->handle());
}

void QWWaylandBackend::waylandOutputSetTitle(QWOutput *output, const char *title)
{
    wlr_wl_output_set_title(output->handle(), title);
}

wl_surface *QWWaylandBackend::waylandOutputGetSurface(QWOutput *output)
{
    return wlr_wl_output_get_surface(output->handle());
}

QWWaylandBackend::QWWaylandBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

#ifdef WLR_HAVE_X11_BACKEND

QWX11Backend *QWX11Backend::get(wlr_backend *handle)
{
    return qobject_cast<QWX11Backend*>(QWBackend::get(handle));
}

QWX11Backend *QWX11Backend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_x11(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWX11Backend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWX11Backend(handle, false);
}

QWX11Backend *QWX11Backend::create(QWDisplay *display, const char *x11Display, QObject *parent)
{
    auto handle = wlr_x11_backend_create(display->handle(), x11Display);
    if (!handle)
        return nullptr;
    return new QWX11Backend(handle, true, parent);
}

QWOutput *QWX11Backend::createOutput()
{
    auto output = wlr_x11_output_create(handle());
    if (!output)
        return nullptr;
    return QWOutput::from(output);
}

bool QWX11Backend::isX11Output(QWOutput *output)
{
    return wlr_output_is_x11(output->handle());
}

void QWX11Backend::x11OutputSetTitle(QWOutput *output, const char *title)
{
    wlr_x11_output_set_title(output->handle(), title);
}

bool QWX11Backend::isX11InputDevice(QWInputDevice *device)
{
    return wlr_input_device_is_x11(device->handle());
}

QWX11Backend::QWX11Backend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

#endif

QWLibinputBackend *QWLibinputBackend::get(wlr_backend *handle)
{
    return qobject_cast<QWLibinputBackend*>(QWBackend::get(handle));
}

QWLibinputBackend *QWLibinputBackend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_libinput(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWLibinputBackend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWLibinputBackend(handle, false);
}

QWLibinputBackend *QWLibinputBackend::create(QWDisplay *display, wlr_session *session, QObject *parent)
{
    auto handle = wlr_libinput_backend_create(display->handle(), session);
    if (!handle)
        return nullptr;
    return new QWLibinputBackend(handle, true, parent);
}

bool QWLibinputBackend::isLibinputDevice(QWInputDevice *device)
{
    return wlr_input_device_is_libinput(device->handle());
}

libinput_device *QWLibinputBackend::getDeviceHandle(QWInputDevice *dev)
{
    return wlr_libinput_get_device_handle(dev->handle());
}

QWLibinputBackend::QWLibinputBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

QWHeadlessBackend *QWHeadlessBackend::get(wlr_backend *handle)
{
    return qobject_cast<QWHeadlessBackend*>(QWBackend::get(handle));
}

QWHeadlessBackend *QWHeadlessBackend::from(wlr_backend *handle)
{
    if (!wlr_backend_is_headless(handle))
        return nullptr;
    auto base = QWBackend::get(handle);
    if (base) {
        auto backend = qobject_cast<QWHeadlessBackend*>(base);
        Q_ASSERT(backend);
        return backend;
    }

    return new QWHeadlessBackend(handle, false);
}

QWHeadlessBackend *QWHeadlessBackend::create(QWDisplay *display, QObject *parent)
{
    auto handle = wlr_headless_backend_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWHeadlessBackend(handle, true, parent);
}

QWOutput *QWHeadlessBackend::addOutput(unsigned int width, unsigned int height)
{
    auto output = wlr_headless_add_output(handle(), width, height);
    if (!output)
        return nullptr;
    return QWOutput::from(output);
}

bool QWHeadlessBackend::isHeadlessOutput(QWOutput *output)
{
    return wlr_output_is_headless(output->handle());
}

QWHeadlessBackend::QWHeadlessBackend(wlr_backend *handle, bool isOwner, QObject *parent)
    : QWBackend(handle, isOwner, parent)
{

}

QW_END_NAMESPACE
