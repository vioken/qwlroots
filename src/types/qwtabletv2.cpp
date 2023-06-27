// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtabletv2.h"

extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
}

static_assert(std::is_same_v<zwp_tablet_pad_v2_button_state_t, std::underlying_type_t<zwp_tablet_pad_v2_button_state>>);

QW_BEGIN_NAMESPACE

QWTabletPadV2Grab *QWTabletPadV2Grab::from(wlr_tablet_pad_v2_grab *handle)
{
    return reinterpret_cast<QWTabletPadV2Grab*>(handle);
}

wlr_tablet_pad_v2_grab *QWTabletPadV2Grab::handle() const
{
    return reinterpret_cast<wlr_tablet_pad_v2_grab*>(const_cast<QWTabletPadV2Grab*>(this));
}

QWTabletToolV2Grab *QWTabletToolV2Grab::from(wlr_tablet_tool_v2_grab *handle)
{
    return reinterpret_cast<QWTabletToolV2Grab*>(handle);
}

wlr_tablet_tool_v2_grab *QWTabletToolV2Grab::handle() const
{
    return reinterpret_cast<wlr_tablet_tool_v2_grab*>(const_cast<QWTabletToolV2Grab*>(this));
}

QW_END_NAMESPACE
