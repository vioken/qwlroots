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
    QW_FUNC_STATIC(input_method_manager_v2, create)
}


class QW_CLASS_OBJECT(input_method_keyboard_grab_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, send_key)
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, send_modifiers)
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, set_keyboard)

protected:
    QW_FUNC_MEMBER(input_method_keyboard_grab_v2, destroy)
}

class QW_CLASS_OBJECT(input_popup_surface_v2)
{
    QW_OBJECT
    Q_OBJECT

public:
    QW_FUNC_MEMBER(input_popup_surface_v2, send_text_input_rectangle)
    QW_FUNC_STATIC(input_popup_surface_v2, try_from_wlr_surface)
}

class QW_CLASS_OBJECT(input_method_v2)
{
    QW_OBJECT
    Q_OBJECT

    QW_SIGNAL(commit, wlr_input_method_v2*)
    QW_SIGNAL(new_popup_surface, wlr_input_popup_surface_v2*)
    QW_SIGNAL(grab_keyboard, wlr_input_method_keyboard_grab_v2*)

public:
    QW_FUNC_MEMBER(input_method_v2, send_activate)
    QW_FUNC_MEMBER(input_method_v2, send_content_type)
    QW_FUNC_MEMBER(input_method_v2, send_deactivate)
    QW_FUNC_MEMBER(input_method_v2, send_done)
    QW_FUNC_MEMBER(input_method_v2, send_surrounding_text)
    QW_FUNC_MEMBER(input_method_v2, send_text_change_cause)
    QW_FUNC_MEMBER(input_method_v2, send_unavailable)
}
QW_END_NAMESPACE
