// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_input_inhibitor.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(input_inhibit_manager)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_SIGNAL(activate)
    QW_SIGNAL(deactivate)

public:
    QW_FUNC_STATIC(input_inhibit_manager, create)
}


QW_END_NAMESPACE
