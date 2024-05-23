// Copyright (C) 2024 WenHao Peng <pengwenhao@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlogging.h"

#include <QDebug>
#include <QLoggingCategory>

extern "C" {
#include <wlr/util/log.h>
}

QW_BEGIN_NAMESPACE

#ifdef QT_DEBUG
    Q_LOGGING_CATEGORY(lcQWLog, "wlroots", QtDebugMsg);
#else
    Q_LOGGING_CATEGORY(lcQWLog, "wlroots", QtInfoMsg);
#endif

static wlr_log_importance logLevel = WLR_DEBUG;

void log_callback(enum wlr_log_importance importance, const char *fmt, va_list args)
{
    if (importance > logLevel)
        return;

    QString message;
    message = QString::vasprintf(fmt, args);

    switch((int)importance) {
    case WLR_ERROR :
        qCCritical(lcQWLog) << message;
        break;

    case WLR_INFO :
        qCInfo(lcQWLog) << message;
        break;

    case WLR_DEBUG :
        qCDebug(lcQWLog) << message;
        break;

    default:
        break;
    }
}

void QWLog::init(Importance importance)
{
    init(importance, nullptr);
}

void QWLog::init(Importance importance, qwl_log_func_t callback)
{
    logLevel = static_cast<wlr_log_importance>(importance);
    wlr_log_init(logLevel, callback ? wlr_log_func_t(callback) : log_callback);
}

QW_END_NAMESPACE
