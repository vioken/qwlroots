// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwglobal.h"
#include <qwxdgforeignregistry.h>

struct wlr_xdg_foreign_v2;
struct wlr_xdg_foreign_registry;
struct wlr_xdg_exported_v2;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXdgForeignRegistry;
class QWXdgForeignv2Private;
class QW_EXPORT QWXdgForeignv2 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgForeignv2)
public:
    static QWXdgForeignv2 *create(QWDisplay *display, QWXdgForeignRegistry *registry);
    static QWXdgForeignv2 *from(wlr_xdg_foreign_v2 *handle);
    static QWXdgForeignv2 *get(wlr_xdg_foreign_v2 *handle);

    inline wlr_xdg_foreign_v2 *handle() const {
        return QWObject::handle<wlr_xdg_foreign_v2>();
    }

Q_SIGNALS:
    void beforeDestroy(QWXdgForeignv2 *self);

private:
    explicit QWXdgForeignv2(wlr_xdg_foreign_v2 *handle, bool isOwner);
    ~QWXdgForeignv2() override = default;
};

class QWXdgExportedv2Private;
class QW_EXPORT QWXdgExportedv2 : public QWXdgForeignExported
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgExportedv2)
public:
    static QWXdgExportedv2 *from(wlr_xdg_exported_v2 *handle);
    static QWXdgExportedv2 *get(wlr_xdg_exported_v2 *handle);

    inline wlr_xdg_exported_v2 *handle() const {
        return QWObject::handle<wlr_xdg_exported_v2>();
    }

private:
    explicit QWXdgExportedv2(wlr_xdg_exported_v2 *handle, bool isOwner);
    ~QWXdgExportedv2() override = default;
};

QW_END_NAMESPACE
