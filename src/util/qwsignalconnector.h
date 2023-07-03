// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QVector>

struct wl_signal;

QW_BEGIN_NAMESPACE

struct Listener;
class QW_EXPORT QWSignalConnector {
public:
    QWSignalConnector();
    ~QWSignalConnector();

    using SlotFun0 = void (*)(void *obj);
    Listener *connect(wl_signal *signal, void *object, SlotFun0 slot);
    using SlotFun1 = void (*)(void *obj, void *signalData);
    Listener *connect(wl_signal *signal, void *object, SlotFun1 slot);
    using SlotFun2 = void (*)(void *obj, void *signalData, void *data);
    Listener *connect(wl_signal *signal, void *object, SlotFun2 slot, void *data);
    template <typename T>
    inline Listener *connect(wl_signal *signal, T *object, void (T::*slot)()) {
        return connect(signal, object, reinterpret_cast<SlotFun0>(*(void**)(&slot)));
    }
    template <typename T, typename T1>
    inline Listener *connect(wl_signal *signal, T *object, void (T::*slot)(T1*)) {
        return connect(signal, object, reinterpret_cast<SlotFun1>(*(void**)(&slot)));
    }
    template <typename T, typename T1, typename T2, typename T3>
    inline Listener *connect(wl_signal *signal, T *object, void (T::*slot)(T1*, T2*), T3 *data) {
        return connect(signal, object, reinterpret_cast<SlotFun2>(*(void**)(&slot)), data);
    }
    void disconnect(Listener *l);
    void disconnect(wl_signal *signal);
    void invalidate();

private:
    QVector<Listener*> listenerList;
};

QW_END_NAMESPACE
