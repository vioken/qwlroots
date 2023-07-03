// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <qwbackendinterface.h>

#include <QObject>
#include <type_traits>

struct wlr_backend;
struct wlr_output;
struct wlr_session;
struct wlr_device;
struct wlr_output_mode;
struct wlr_drm_lease;
struct wl_display;
struct wl_seat;
struct wl_surface;
struct libinput_device;

typedef struct _drmModeModeInfo drmModeModeInfo;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWOutput;
class QWInputDevice;
class QWBackendPrivate;
class QW_EXPORT QWBackend : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWBackend)
public:
    ~QWBackend();

    inline wlr_backend *handle() const {
        return QWObject::handle<wlr_backend>();
    }

    static QWBackend *get(wlr_backend *handle);
    static QWBackend *from(wlr_backend *handle);
    static QWBackend *autoCreate(QWDisplay *display, QObject *parent = nullptr);
    template<class Interface, typename... Args>
    inline static typename std::enable_if<std::is_base_of<QWBackendInterface, Interface>::value, QWBackend*>::type
    create(Args&&... args) {
        Interface *i = new Interface();
        i->QWBackendInterface::template init<Interface>(std::forward<Args>(args)...);
        return new QWBackend(i->handle(), true, nullptr);
    }

    clockid_t presentationClock() const;
    int drmFd() const;

public Q_SLOTS:
    bool start();

Q_SIGNALS:
    void beforeDestroy(QWBackend *self);
    void newInput(QWInputDevice *device);
    void newOutput(QWOutput *output);

protected:
    QWBackend(QWBackendPrivate &dd, QObject *parent = nullptr);
    QWBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

using wlr_multi_backend_iterator_func_t = void (*)(struct wlr_backend *backend, void *data);

class QW_EXPORT QWMultiBackend : public QWBackend
{
    Q_OBJECT
public:
    static QWMultiBackend *get(wlr_backend *handle);
    static QWMultiBackend *from(wlr_backend *handle);
    static QWMultiBackend *create(QWDisplay *display, QObject *parent = nullptr);

    bool add(QWBackend *backend);
    void remove(QWBackend *backend);

    bool isEmpty() const;
    void forEachBackend(wlr_multi_backend_iterator_func_t iterator, void *userData);

private:
    friend class QWBackendPrivate;
    QWMultiBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

typedef uint32_t wl_output_transform_t;
class QW_EXPORT QWDrmBackend : public QWBackend
{
    Q_OBJECT
public:
    static QWDrmBackend *get(wlr_backend *handle);
    static QWDrmBackend *from(wlr_backend *handle);
    static QWDrmBackend *create(QWDisplay *display, wlr_session *session, wlr_device *dev, QWBackend *parent);

    static bool isDrmOutput(QWOutput *output);
    static uint32_t connectorGetId(QWOutput *output);
    static wlr_output_mode *connectorAddMode(QWOutput *output, const drmModeModeInfo *mode);
    static wl_output_transform_t connectorGetPanelOrientation(QWOutput *output);

    static wlr_drm_lease *createLease(wlr_output **outputs, size_t outputCount, int *leaseFd);
    static void terminateLease(wlr_drm_lease *lease);

    int getNonMasterFd() const;

private:
    friend class QWBackendPrivate;
    QWDrmBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

class QW_EXPORT QWWaylandBackend : public QWBackend
{
    Q_OBJECT
public:
    static QWWaylandBackend *get(wlr_backend *handle);
    static QWWaylandBackend *from(wlr_backend *handle);
    static QWWaylandBackend *create(QWDisplay *display, const char *remote, QObject *parent = nullptr);

    wl_display *getRemoteDisplay() const;
    QWOutput *createOutput();

    static bool isWaylandInputDevice(QWInputDevice *device);

    static bool isWaylandOutput(QWOutput *output);
    static void waylandOutputSetTitle(QWOutput *output, const char *title);
    static wl_surface *waylandOutputGetSurface(QWOutput *output);

private:
    friend class QWBackendPrivate;
    QWWaylandBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

class QW_EXPORT QWX11Backend : public QWBackend
{
    Q_OBJECT
public:
    static QWX11Backend *get(wlr_backend *handle);
    static QWX11Backend *from(wlr_backend *handle);
    static QWX11Backend *create(QWDisplay *display, const char *x11Display, QObject *parent = nullptr);

    QWOutput *createOutput();

    static bool isX11Output(QWOutput *output);
    static void x11OutputSetTitle(QWOutput *output, const char *title);
    static bool isX11InputDevice(QWInputDevice *device);

private:
    friend class QWBackendPrivate;
    QWX11Backend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

class QW_EXPORT QWLibinputBackend : public QWBackend
{
    Q_OBJECT
public:
    static QWLibinputBackend *get(wlr_backend *handle);
    static QWLibinputBackend *from(wlr_backend *handle);
    static QWLibinputBackend *create(QWDisplay *display, wlr_session *session, QObject *parent = nullptr);

    static bool isLibinputDevice(QWInputDevice *device);
    static libinput_device *getDeviceHandle(QWInputDevice *dev);

private:
    friend class QWBackendPrivate;
    QWLibinputBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

class QW_EXPORT QWHeadlessBackend : public QWBackend
{
    Q_OBJECT
public:
    static QWHeadlessBackend *get(wlr_backend *handle);
    static QWHeadlessBackend *from(wlr_backend *handle);
    static QWHeadlessBackend *create(QWDisplay *display, QObject *parent = nullptr);

    QWOutput *addOutput(unsigned int width, unsigned int height);
    static bool isHeadlessOutput(QWOutput *output);

private:
    friend class QWBackendPrivate;
    QWHeadlessBackend(wlr_backend *handle, bool isOwner, QObject *parent = nullptr);
};

QW_END_NAMESPACE
