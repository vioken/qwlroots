#include "qwvirtualkeyboardv1.h"
#include "qwkeyboard_p.h"
#include "qwinputdevice.h"
#include "private/qwglobal_p.h"
extern "C" {
#include <wlr/types/wlr_virtual_keyboard_v1.h>
QW_FUNC_MEMBER(input, device_get_virtual_keyboard)
