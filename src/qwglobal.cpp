// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwglobal.h"

QW_BEGIN_NAMESPACE

QWObject::QWObject(QWObjectPrivate &dd)
    : qw_d_ptr(&dd)
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

QW_END_NAMESPACE
