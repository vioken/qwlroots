// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwglobal.h"

QW_BEGIN_NAMESPACE

QWObject::QWObject(QWObjectPrivate &dd, QWObject *)
    : qw_d_ptr(&dd)
{

}

QWObject::~QWObject()
{

}

QWObjectPrivate::~QWObjectPrivate()
{

}

QWObjectPrivate::QWObjectPrivate(void *handle, QWObject *qq)
    : m_handle(handle)
    , q_ptr(qq)
{

}

QW_END_NAMESPACE
