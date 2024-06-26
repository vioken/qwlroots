#include "qwseat.h"
#include "qwdisplay.h"
#include "qwkeyboard.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_seat.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_pointer.h>
#define static
#include <wlr/types/wlr_compositor.h>
#undef static
#if WLR_VERSION_MINOR < 18
#endif
QW_SIGNAL(pointer_grab_begin, arguments?)
QW_SIGNAL(pointer_grab_end, arguments?)
QW_SIGNAL(keyboard_grab_begin, arguments?)
QW_SIGNAL(keyboard_grab_end, arguments?)
QW_SIGNAL(touch_grab_begin, arguments?)
QW_SIGNAL(touch_grab_end, arguments?)
QW_SIGNAL(request_set_cursor, arguments?)
QW_SIGNAL(request_set_selection, arguments?)
QW_SIGNAL(set_selection, arguments?)
QW_SIGNAL(request_set_primary_selection, arguments?)
QW_SIGNAL(set_primary_selection, arguments?)
QW_SIGNAL(request_start_drag, arguments?)
QW_SIGNAL(start_drag, arguments?)
QW_FUNC_MEMBER(seat, create)
QW_FUNC_MEMBER(seat, set_keyboard)
QW_FUNC_MEMBER(seat, get_keyboard)
QW_FUNC_MEMBER(seat, set_capabilities)
QW_FUNC_MEMBER(seat, set_selection)
QW_FUNC_MEMBER(seat, keyboard_clear_focus)
QW_FUNC_MEMBER(seat, keyboard_end_grab)
QW_FUNC_MEMBER(seat, keyboard_enter)
QW_FUNC_MEMBER(seat, keyboard_has_grab)
QW_FUNC_MEMBER(seat, keyboard_notify_clear_focus)
QW_FUNC_MEMBER(seat, keyboard_notify_enter)
QW_FUNC_MEMBER(seat, keyboard_notify_key)
QW_FUNC_MEMBER(seat, keyboard_notify_modifiers)
QW_FUNC_MEMBER(seat, keyboard_send_key)
QW_FUNC_MEMBER(seat, keyboard_send_modifiers)
QW_FUNC_MEMBER(seat, keyboard_start_grab)
QW_FUNC_MEMBER(seat, pointer_end_grab)
QW_FUNC_MEMBER(seat, pointer_enter)
QW_FUNC_MEMBER(seat, pointer_has_grab)
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, pointer_notify_axis)
#else
QW_FUNC_MEMBER(seat, pointer_notify_axis)
#endif
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, pointer_notify_button)
#else
QW_FUNC_MEMBER(seat, pointer_notify_button)
#endif
QW_FUNC_MEMBER(seat, pointer_notify_clear_focus)
QW_FUNC_MEMBER(seat, pointer_notify_enter)
QW_FUNC_MEMBER(seat, pointer_notify_frame)
QW_FUNC_MEMBER(seat, pointer_notify_motion)
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, pointer_send_axis)
#else
QW_FUNC_MEMBER(seat, pointer_send_axis)
#endif
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, pointer_send_button)
#else
QW_FUNC_MEMBER(seat, pointer_send_button)
#endif
QW_FUNC_MEMBER(seat, pointer_send_frame)
QW_FUNC_MEMBER(seat, pointer_send_motion)
QW_FUNC_MEMBER(seat, pointer_start_grab)
QW_FUNC_MEMBER(seat, pointer_surface_has_focus)
QW_FUNC_MEMBER(seat, pointer_warp)
QW_FUNC_MEMBER(seat, pointer_clear_focus)
QW_FUNC_MEMBER(seat, set_name)
QW_FUNC_MEMBER(seat, touch_end_grab)
QW_FUNC_MEMBER(seat, touch_get_point)
QW_FUNC_MEMBER(seat, touch_has_grab)
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, touch_notify_cancel)
#else
QW_FUNC_MEMBER(seat, touch_notify_cancel)
#endif
QW_FUNC_MEMBER(seat, touch_notify_down)
QW_FUNC_MEMBER(seat, touch_notify_frame)
QW_FUNC_MEMBER(seat, touch_notify_motion)
QW_FUNC_MEMBER(seat, touch_notify_up)
QW_FUNC_MEMBER(seat, touch_num_points)
QW_FUNC_MEMBER(seat, touch_point_clear_focus)
QW_FUNC_MEMBER(seat, touch_point_focus)
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(seat, touch_send_cancel)
#else
QW_FUNC_MEMBER(seat, touch_send_cancel)
#endif
QW_FUNC_MEMBER(seat, touch_send_down)
QW_FUNC_MEMBER(seat, touch_send_frame)
QW_FUNC_MEMBER(seat, touch_send_motion)
QW_FUNC_MEMBER(seat, touch_send_up)
QW_FUNC_MEMBER(seat, touch_start_grab)
QW_FUNC_MEMBER(seat, validate_pointer_grab_serial)
QW_FUNC_MEMBER(seat, validate_touch_grab_serial)
QW_FUNC_MEMBER(surface, accepts_touch)
