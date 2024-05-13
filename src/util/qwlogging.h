// Copyright (C) 2024 WenHao Peng <pengwenhao@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <stdio.h>

QW_BEGIN_NAMESPACE

class QW_EXPORT QWLog {
public:
    enum Importance {
        QWL_SILENT = 0,
        QWL_ERROR = 1,
        QWL_INFO = 2,
        QWL_DEBUG = 3,
        QWL_LOG_IMPORTANCE_LAST,
    };

    typedef void (*qwl_log_func_t)(enum Importance importance,
                                   const char *fmt, va_list args);

    static void init(Importance importance = QWL_DEBUG);
    static void init(Importance importance, qwl_log_func_t callback);
};

QW_END_NAMESPACE
