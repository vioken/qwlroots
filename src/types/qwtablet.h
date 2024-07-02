// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(tablet)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(axis, wlr_tablet_tool_axis_event*)
    QW_SIGNAL(proximity, wlr_tablet_tool_proximity_event*)
    QW_SIGNAL(tip, wlr_tablet_tool_tip_event*)
    QW_SIGNAL(button, wlr_tablet_tool_button_event*)

public:
    QW_FUNC_STATIC(tablet, from_input_device)
};

QW_END_NAMESPACE
