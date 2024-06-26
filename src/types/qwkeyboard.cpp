#include "qwkeyboard.h"
#include "qwkeyboard_p.h"
extern "C" {
#include <wlr/types/wlr_keyboard.h>
QW_SIGNAL(key, arguments?)
QW_SIGNAL(modifiers, arguments?)
QW_SIGNAL(keymap, arguments?)
QW_SIGNAL(repeat_info, arguments?)
QW_FUNC_MEMBER(keyboard, from_input_device)
QW_FUNC_MEMBER(keyboard, get_modifiers)
QW_FUNC_MEMBER(keyboard, set_keymap)
QW_FUNC_MEMBER(keyboard, set_repeat_info)
