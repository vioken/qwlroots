// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_presentation_time.h>
}

QW_BEGIN_NAMESPACE


class QW_CLASS_OBJECT(presentation)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_STATIC(presentation, create)

    QW_FUNC_MEMBER(presentation, surface_sampled)
    QW_FUNC_MEMBER(presentation, surface_textured_on_output)
    QW_FUNC_MEMBER(presentation, surface_scanned_on_output)
};

class QW_CLASS_REINTERPRET_CAST(presentation_event)
{
public:
    QW_FUNC_MEMBER(presentation_event, from_output)
};

class QW_CLASS_REINTERPRET_CAST(presentation_feedback)
{
public:
    QW_FUNC_MEMBER(presentation_event, send_presented)
    QW_FUNC_MEMBER(presentation_event, destroy)
};

QW_END_NAMESPACE
