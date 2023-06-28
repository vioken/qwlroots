// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_xdg_activation_v1;
struct wlr_xdg_activation_token_v1;
struct wlr_xdg_activation_v1_request_activate_event;

QW_BEGIN_NAMESPACE
class QWXdgActivationTokenV1Private;
class QW_EXPORT QWXdgActivationTokenV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgActivationTokenV1)
public:
    ~QWXdgActivationTokenV1() = default;

    inline wlr_xdg_activation_token_v1 *handle() const {
        return QWObject::handle<wlr_xdg_activation_token_v1>();
    }

    static QWXdgActivationTokenV1 *get(wlr_xdg_activation_token_v1 *handle);
    static QWXdgActivationTokenV1 *from(wlr_xdg_activation_token_v1 *handle);

    const char *getName() const;

Q_SIGNALS:
    void beforeDestroy(QWXdgActivationTokenV1 *self);

private:
    QWXdgActivationTokenV1(wlr_xdg_activation_token_v1 *handle, bool isOwner);
};

class QWDisplay;
class QWXdgActivationV1Private;
class QW_EXPORT QWXdgActivationV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWXdgActivationV1)
public:
    inline wlr_xdg_activation_v1 *handle() const {
        return QWObject::handle<wlr_xdg_activation_v1>();
    }

    static QWXdgActivationV1 *get(wlr_xdg_activation_v1 *handle);
    static QWXdgActivationV1 *from(wlr_xdg_activation_v1 *handle);
    static QWXdgActivationV1 *create(QWDisplay *display);

    QWXdgActivationTokenV1 *createToken();
    QWXdgActivationTokenV1 *findToken(const char *token_str) const;
    QWXdgActivationTokenV1 *addToken(const char *token_str);

Q_SIGNALS:
    void beforeDestroy(QWXdgActivationV1 *self);
    void requestActivate(wlr_xdg_activation_v1_request_activate_event *);
#if WLR_VERSION_MINOR > 16
    void newToken(QWXdgActivationTokenV1 *);
#endif

private:
    QWXdgActivationV1(wlr_xdg_activation_v1 *handle, bool isOwner);
    ~QWXdgActivationV1() = default;
};

QW_END_NAMESPACE
