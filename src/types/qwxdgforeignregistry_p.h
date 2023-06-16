// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qwsignalconnector.h"
#include "qwxdgforeignregistry.h"

#include <QHash>

QW_BEGIN_NAMESPACE

class QWXdgForeignExportedPrivate : public QWObjectPrivate
{
public:
    QWXdgForeignExportedPrivate(wlr_xdg_foreign_exported *handle, bool isOwner, QWXdgForeignExported *qq);
    virtual ~QWXdgForeignExportedPrivate() override;

    inline void destroy();
    void on_destroy();

    static QHash<void*, QWXdgForeignExported*> map;
    QW_DECLARE_PUBLIC(QWXdgForeignExported)
    QWSignalConnector sc;
};

QW_END_NAMESPACE
