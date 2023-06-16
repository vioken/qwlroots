// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_switch;
struct wlr_switch_impl;

QW_BEGIN_NAMESPACE

class QWSwitchInterface : public QWInterface {
    friend class QWSwitch;
public:
    virtual const char* name() = 0;

    inline wlr_switch *handle() const {
        return QWInterface::handle<wlr_switch>();
    }

protected:
    void notifyToggle() const;

    template<class T>
    inline void init(const char* name)
    {
        init(getFuncMagicKey<T>(), name);
    }

    virtual void init(FuncMagicKey funMagicKey, const char* name);
};

QW_END_NAMESPACE
