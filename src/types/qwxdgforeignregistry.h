// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_xdg_foreign_registry;
struct wlr_xdg_foreign_exported;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWXdgForeignExported;
class QWXdgForeignRegistryPrivate;
class QW_EXPORT QWXdgForeignRegistry : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgForeignRegistry)
public:
    static QWXdgForeignRegistry *create(QWDisplay *display);
    static QWXdgForeignRegistry *from(wlr_xdg_foreign_registry *handle);
    static QWXdgForeignRegistry *get(wlr_xdg_foreign_registry *handle);

    QWXdgForeignExported *findByHandle(const char *handle);
    inline wlr_xdg_foreign_registry *handle() const {
        return QWObject::handle<wlr_xdg_foreign_registry>();
    }

Q_SIGNALS:
    void beforeDestroy(QWXdgForeignRegistry *self);

private:
    explicit QWXdgForeignRegistry(wlr_xdg_foreign_registry *handle, bool isOwner);
    ~QWXdgForeignRegistry() override = default;
};

class QWXdgForeignExportedPrivate;
class QW_EXPORT QWXdgForeignExported : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgForeignExported)
public:
    static QWXdgForeignExported *from(wlr_xdg_foreign_exported *handle);
    static QWXdgForeignExported *get(wlr_xdg_foreign_exported *handle);

    inline wlr_xdg_foreign_exported *handle() const {
        return QWObject::handle<wlr_xdg_foreign_exported>();
    }

    bool init(QWXdgForeignRegistry *registry);
    void finish();

Q_SIGNALS:
    void beforeDestroy(QWXdgForeignExported *self);

protected:
    QWXdgForeignExported(QWObjectPrivate &dd);
    virtual ~QWXdgForeignExported() override = default;

private:
    explicit QWXdgForeignExported(wlr_xdg_foreign_exported *handle, bool isOwner);
};

QW_END_NAMESPACE
