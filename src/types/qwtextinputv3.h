// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(text_input_v3)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(enable, wlr_text_input_v3*)
    QW_SIGNAL(commit, wlr_text_input_v3*)
    QW_SIGNAL(disable, wlr_text_input_v3*)

public:
    QW_FUNC_STATIC(text_input_manager_v3, create)
    QW_FUNC_MEMBER(text_input_v3, send_leave)
    QW_FUNC_MEMBER(text_input_v3, send_preedit_string)
    QW_FUNC_MEMBER(text_input_v3, send_commit_string)
    QW_FUNC_MEMBER(text_input_v3, send_delete_surrounding_text)
    QW_FUNC_MEMBER(text_input_v3, send_done)
};

class QW_CLASS_OBJECT(text_input_manager_v3)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(text_input, wlr_text_input_v3*)

public:
    QW_FUNC_STATIC(text_input_manager_v3, create)
};

QW_END_NAMESPACE
