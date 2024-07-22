// Copyright (C) 2023 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwinterface.h>

extern "C" {
#define static
#include <wlr/types/wlr_output.h>
#undef static
#include <wlr/interfaces/wlr_output.h>
}

QW_BEGIN_NAMESPACE

template<typename Derive>
class QW_CLASS_INTERFACE(output)
{
    QW_INTERFACE_INIT(output)

public:
    QW_INTERFACE_FUNC_STATIC(output, init, void, wlr_output *output, wlr_backend *backend,
                             const wlr_output_impl *impl, wl_display *display,
                             const wlr_output_state *state)

    QW_INTERFACE(set_cursor)
    QW_INTERFACE(move_cursor)
    QW_INTERFACE(test)
    QW_INTERFACE(commit)
    QW_INTERFACE(get_gamma_size)
    QW_INTERFACE(get_cursor_formats)
    QW_INTERFACE(get_cursor_size)
    QW_INTERFACE(get_primary_formats)
};

QW_END_NAMESPACE
