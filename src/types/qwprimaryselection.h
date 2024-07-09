// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_primary_selection.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(primary_selection_source)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(seat, request_set_primary_selection)
    QW_FUNC_STATIC(seat, set_primary_selection)

    QW_FUNC_MEMBER(primary_selection_source, init)
    QW_FUNC_MEMBER(primary_selection_source, send)

protected:
    QW_FUNC_MEMBER(primary_selection_source, destroy)
};
QW_END_NAMESPACE
