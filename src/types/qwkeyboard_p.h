// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

// WARNING: This file is not part of the QWlroots API.

#include "qw_global.h"
#include "qwinputdevice_p.h"
#include "util/qwsignalconnector.h"

QW_BEGIN_NAMESPACE

class QWKeyboard;

class QWKeyboardPrivate : public QWInputDevicePrivate
{
public:
    QWKeyboardPrivate(wlr_keyboard *handle, bool isOwner, QWKeyboard *qq);
    ~QWKeyboardPrivate() override;

    void on_key(void *);
    void on_modifiers(void *);
    void on_keymap(void *);
    void on_repeat_info(void *);

    QW_DECLARE_PUBLIC(QWKeyboard)
};

QW_END_NAMESPACE
