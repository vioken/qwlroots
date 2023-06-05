// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

struct wlr_tablet_pad;
struct wlr_tablet_pad_impl;
struct wlr_tablet_tool;

QW_BEGIN_NAMESPACE
class QWTabletPad;
class QWTabletPadInterface : public QWInterface {
    friend class QWTabletPad;
public:
    virtual const char* name() = 0;

    inline wlr_tablet_pad *handle() const {
        return QWInterface::handle<wlr_tablet_pad>();
    }

protected:
    void notifyButton();
    void notifyRing();
    void notifyStrip();
    void notifyAttachTablet(wlr_tablet_tool *table);

    template<class T>
    inline void init(const char* name)
    {
        init(getFuncMagicKey<T>(), name);
    }

    virtual void init(FuncMagicKey funMagicKey, const char* name);
};

QW_END_NAMESPACE
