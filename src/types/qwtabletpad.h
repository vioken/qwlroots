// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_tablet_pad.h>
#include <wlr/types/wlr_tablet_tool.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(tablet_pad)
{
    QW_OBJECT
    Q_OBJECT
public:
    QW_SIGNAL(button, wlr_tablet_pad_button_event*)
    QW_SIGNAL(ring, wlr_tablet_pad_ring_event*)
    QW_SIGNAL(strip, wlr_tablet_pad_strip_event*)
    QW_SIGNAL(attach_tablet, wlr_tablet_tool*)

public:
    QW_FUNC_STATIC(tablet_pad, from_input_device)
};

QW_END_NAMESPACE
