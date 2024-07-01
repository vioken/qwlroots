// Copyright (C) 2024 WenHao Peng <pengwenhao@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwlogging.h"

#include <QDebug>
#include <QLoggingCategory>

extern "C" {
#include <wlr/util/log.h>
}

QW_BEGIN_NAMESPACE

void qw_log::init(Importance importance)
{
    init(importance, nullptr);
}

void qw_log::init(Importance importance, qwl_log_func_t callback)
{
    logLevel = static_cast<wlr_log_importance>(importance);
QW_FUNC_MEMBER(log, func_t)
}

QW_END_NAMESPACE
