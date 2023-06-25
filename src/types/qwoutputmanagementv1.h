// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_output_manager_v1;
struct wlr_output_configuration_v1;
struct wlr_output_configuration_head_v1;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWOutput;
class QW_EXPORT QWOutputConfigurationV1
{
public:
    QWOutputConfigurationV1() = delete;
    ~QWOutputConfigurationV1() = delete;

    void operator delete(QWOutputConfigurationV1 *p, std::destroying_delete_t);

    wlr_output_configuration_v1 *handle() const;

    static QWOutputConfigurationV1 *from(wlr_output_configuration_v1 *handle);
    static QWOutputConfigurationV1 *create();

    void sendFailed();
    void sendSucceeded();
};

class QW_EXPORT QWOutputConfigurationHeadV1
{
public:
    QWOutputConfigurationHeadV1() = delete;
    ~QWOutputConfigurationHeadV1() = delete;

    wlr_output_configuration_head_v1 *handle() const;

    static QWOutputConfigurationHeadV1 *from(wlr_output_configuration_head_v1 *handle);
    static QWOutputConfigurationHeadV1 *create(QWOutputConfigurationV1 *config, QWOutput *output);
};

class QWOutputManagerV1Private;
class QW_EXPORT QWOutputManagerV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWOutputManagerV1)
public:
    inline wlr_output_manager_v1 *handle() const {
        return QWObject::handle<wlr_output_manager_v1>();
    }

    static QWOutputManagerV1 *get(wlr_output_manager_v1 *handle);
    static QWOutputManagerV1 *from(wlr_output_manager_v1 *handle);
    static QWOutputManagerV1 *create(QWDisplay *display);

    void setConfiguration(QWOutputConfigurationV1 *config);

Q_SIGNALS:
    void beforeDestroy(QWOutputManagerV1 *self);
    void apply(QWOutputConfigurationV1 *config);
    void test(QWOutputConfigurationV1 *config);

private:
    QWOutputManagerV1(wlr_output_manager_v1 *handle, bool isOwner);
    ~QWOutputManagerV1() = default;
};

QW_END_NAMESPACE

