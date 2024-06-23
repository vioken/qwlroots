// Copyright (C) 2024 JiDe Zhang <zhangjide@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "qw_global.h"
#include "util/qwsignalconnector.h"

QW_BEGIN_NAMESPACE

class QWWrapObjectPrivate : public QWObjectPrivate
{
public:
    explicit QWWrapObjectPrivate(void *handle, bool isOwner, QWWrapObject *qq,
                                 wl_signal *destroy_signal = nullptr,
                                 std::function<void(void*)> destroy_function = nullptr);

    ~QWWrapObjectPrivate();

    template <typename Func>
    static inline typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == 1, void(*)(void*)>::type
    toDestroyFunction(Func fun) {
        return reinterpret_cast<void(*)(void*)>(fun);
    }

protected:
    void on_destroy(void *);
    void destroy();

    QWSignalConnector sc;
    std::function<void(void*)> wlr_destroy_function;
    static QHash<void*, QWWrapObject*> map;

    QW_DECLARE_PUBLIC(QWWrapObject)
};

QW_END_NAMESPACE
