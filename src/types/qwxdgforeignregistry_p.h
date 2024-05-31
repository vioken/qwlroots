// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qwsignalconnector.h"
#include "qwxdgforeignregistry.h"
#include "private/qwglobal_p.h"

#include <QHash>

QW_BEGIN_NAMESPACE

class QWXdgForeignExportedPrivate : public QWWrapObjectPrivate
{
public:
    QWXdgForeignExportedPrivate(wlr_xdg_foreign_exported *handle, bool isOwner, QWXdgForeignExported *qq);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignExported)
};

QW_END_NAMESPACE
