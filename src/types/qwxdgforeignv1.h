// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwglobal.h"
#include <qwxdgforeignregistry.h>

struct wlr_xdg_foreign_v1;
struct wlr_xdg_foreign_registry;
struct wlr_xdg_exported_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXdgForeignRegistry;
class QWXdgForeignV1Private;
class QW_EXPORT QWXdgForeignV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgForeignV1)
public:
    static QWXdgForeignV1 *create(QWDisplay *display, QWXdgForeignRegistry *registry);
    static QWXdgForeignV1 *from(wlr_xdg_foreign_v1 *handle);
    static QWXdgForeignV1 *get(wlr_xdg_foreign_v1 *handle);

    inline wlr_xdg_foreign_v1 *handle() const {
        return QWObject::handle<wlr_xdg_foreign_v1>();
    }

Q_SIGNALS:
    void beforeDestroy(QWXdgForeignV1 *self);

private:
    explicit QWXdgForeignV1(wlr_xdg_foreign_v1 *handle, bool isOwner);
    ~QWXdgForeignV1() override = default;
};

class QWXdgExportedV1Private;
class QW_EXPORT QWXdgExportedV1 : public QWXdgForeignExported
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgExportedV1)
public:
    static QWXdgExportedV1 *from(wlr_xdg_exported_v1 *handle);
    static QWXdgExportedV1 *get(wlr_xdg_exported_v1 *handle);

    inline wlr_xdg_exported_v1 *handle() const {
        return QWObject::handle<wlr_xdg_exported_v1>();
    }

private:
    explicit QWXdgExportedV1(wlr_xdg_exported_v1 *handle, bool isOwner);
    ~QWXdgExportedV1() override = default;
};

QW_END_NAMESPACE
