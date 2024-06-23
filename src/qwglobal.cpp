// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qw_global.h"
#include "private/qwglobal_p.h"

#include <QHash>

QW_BEGIN_NAMESPACE

QWObject::QWObject(QWObjectPrivate &dd)
    : qw_d_ptr(&dd)
    , m_data(std::make_pair(nullptr, nullptr))
{

}

QWObject::~QWObject()
{

}

QWObjectPrivate::~QWObjectPrivate()
{

}

QWObjectPrivate::QWObjectPrivate(void *handle, bool isOwner, QWObject *qq)
    : q_ptr(qq)
    , m_handle(handle)
    , isHandleOwner(isOwner)
{

}

void QWObject::setData(void *owner, void *data)
{
    if (m_data.first && owner) {
        Q_ASSERT(m_data.first == owner);
    }

    m_data = std::make_pair(owner, data);
}

QHash<void*, QWWrapObject*> QWWrapObjectPrivate::map;
QWWrapObjectPrivate::QWWrapObjectPrivate(void *handle, bool isOwner, QWWrapObject *qq,
                                         wl_signal *destroy_signal,
                                         std::function<void (void *)> destroy_function)
    : QWObjectPrivate(handle, isOwner, qq)
    , wlr_destroy_function(destroy_function)
{
    Q_ASSERT(!map.contains(handle));
    map.insert(handle, qq);

    if (destroy_signal)
        sc.connect(destroy_signal, this, &QWWrapObjectPrivate::on_destroy);
}

QWWrapObjectPrivate::~QWWrapObjectPrivate()
{
    if (m_handle && isHandleOwner) {
        if (wlr_destroy_function)
            wlr_destroy_function(m_handle);
        else
            qFatal("QWWrapObject(%p) can't to destroy, maybe its ownership is wl_display.",
                   static_cast<void*>(q_func()));
    }
}

void QWWrapObjectPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWWrapObjectPrivate::destroy()
{
    Q_ASSERT(m_handle);
    Q_EMIT q_func()->beforeDestroy();

    Q_ASSERT(map.contains(m_handle));
    map.remove(m_handle);

    sc.invalidate();
}

QWWrapObject::QWWrapObject(QWWrapObjectPrivate &dd, QObject *parent)
    : QObject(parent)
    , QWObject(dd)
{

}

QWWrapObject::~QWWrapObject()
{
    Q_D(QWWrapObject);
    if (d->m_handle)
        d->destroy();
}

QW_END_NAMESPACE
