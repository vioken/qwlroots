// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwglobal.h"

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
    if (m_data.first) {
        Q_ASSERT(m_data.first == owner);
    }

    m_data = std::make_pair(owner, data);
}

QW_END_NAMESPACE
