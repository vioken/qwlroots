// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwobject.h>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
#include <wlr/util/box.h>
}
QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(input_method_manager_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wl_display *display
    QW_FUNC_STATIC(input_method_manager_v2, create)
};

class QW_CLASS_OBJECT(input_method_keyboard_grab_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    // uint32_t time, uint32_t key, uint32_t state
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, send_key)
    // wlr_keyboard_modifiers *modifiers
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, send_modifiers)
    // wlr_keyboard *keyboard
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, set_keyboard)

protected:
    // Empty parameters
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, destroy)
};

class QW_CLASS_OBJECT(input_popup_surface_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    // wlr_box *sbox
    QW_FUNC_MEMBER(input_popup_surface_v2, send_text_input_rectangle)
    // wlr_surface *surface
    QW_FUNC_STATIC(input_popup_surface_v2, try_from_wlr_surface)
};

class QW_CLASS_OBJECT(input_method_v2)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(commit, wlr_input_method_v2*)
    QW_SIGNAL(new_popup_surface, wlr_input_popup_surface_v2*)
    QW_SIGNAL(grab_keyboard, wlr_input_method_keyboard_grab_v2*)

public:
    // Empty parameters
    QW_FUNC_MEMBER(input_method_v2, send_activate)
    // uint32_t hint, uint32_t purpose
    QW_FUNC_MEMBER(input_method_v2, send_content_type)
    // Empty parameters
    QW_FUNC_MEMBER(input_method_v2, send_deactivate)
    // Empty parameters
    QW_FUNC_MEMBER(input_method_v2, send_done)
    // const char *text, uint32_t cursor, uint32_t anchor
    QW_FUNC_MEMBER(input_method_v2, send_surrounding_text)
    // uint32_t cause
    QW_FUNC_MEMBER(input_method_v2, send_text_change_cause)
    // Empty parameters
    QW_FUNC_MEMBER(input_method_v2, send_unavailable)
};

QW_END_NAMESPACE
