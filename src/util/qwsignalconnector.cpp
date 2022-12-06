// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwsignalconnector.h"

#include <wayland-server-core.h>

QW_BEGIN_NAMESPACE

struct Listener {
    wl_signal *signal;
    wl_listener l;
    void *object;
    union {
        QWSignalConnector::SlotFun0 slot0;
        QWSignalConnector::SlotFun1 slot1;
        QWSignalConnector::SlotFun2 slot2;
    };
    void *data;
};

static void callSlot0(wl_listener *wl_listener, void *) {
    Listener *listener = wl_container_of(wl_listener, listener, l);
    listener->slot0(listener->object);
}

static void callSlot1(wl_listener *wl_listener, void *data) {
    Listener *listener = wl_container_of(wl_listener, listener, l);
    listener->slot1(listener->object, data);
}

static void callSlot2(wl_listener *wl_listener, void *data) {
    Listener *listener = wl_container_of(wl_listener, listener, l);
    listener->slot2(listener->object, data, listener->data);
}

QWSignalConnector::QWSignalConnector()
{
    listenerList.reserve(1);
}

QWSignalConnector::~QWSignalConnector()
{
    invalidate();
}

Listener *QWSignalConnector::connect(wl_signal *signal, void *object, SlotFun0 slot)
{
    Listener *l = new Listener;
    listenerList.push_back(l);

    l->signal = signal;
    l->l.notify = callSlot0;
    l->object = object;
    l->slot0 = slot;
    wl_signal_add(signal, &l->l);
    return l;
}

Listener *QWSignalConnector::connect(wl_signal *signal, void *object, SlotFun1 slot)
{
    Listener *l = new Listener;
    listenerList.push_back(l);

    l->signal = signal;
    l->l.notify = callSlot1;
    l->object = object;
    l->slot1 = slot;
    wl_signal_add(signal, &l->l);
    return l;
}

Listener *QWSignalConnector::connect(wl_signal *signal, void *object, SlotFun2 slot, void *data)
{
    Q_ASSERT(data);
    Listener *l = new Listener;
    listenerList.push_back(l);

    l->signal = signal;
    l->l.notify = callSlot2;
    l->object = object;
    l->slot2 = slot;
    l->data = data;
    wl_signal_add(signal, &l->l);
    return l;
}

void QWSignalConnector::disconnect(Listener *l)
{
    Q_ASSERT(listenerList.contains(l));
    wl_list_remove(&l->l.link);
    delete l;
    listenerList.removeOne(l);
}

void QWSignalConnector::disconnect(wl_signal *signal)
{
    auto tmpList = listenerList;
    auto begin = tmpList.begin();
    while (begin != tmpList.end()) {
        Listener *l = *begin;
        ++begin;

        if (signal == l->signal)
            disconnect(l);
    }
}

void QWSignalConnector::invalidate()
{
    auto tmpList = listenerList;
    listenerList.clear();
    auto begin = tmpList.begin();
    while (begin != tmpList.end()) {
        Listener *l = *begin;
        wl_list_remove(&l->l.link);
        ++begin;
        delete l;
    }
}

QW_END_NAMESPACE
