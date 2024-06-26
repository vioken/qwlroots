#include "qwkeyboardgroup.h"
#include "private/qwglobal_p.h"
#include "qwkeyboard.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_keyboard_group.h>
QW_SIGNAL(enter, arguments?)
QW_SIGNAL(leave, arguments?)
QW_FUNC_MEMBER(keyboard, group_create)
QW_FUNC_MEMBER(keyboard, group_from_wlr_keyboard)
QW_FUNC_MEMBER(keyboard, group_add_keyboard)
QW_FUNC_MEMBER(keyboard, group_remove_keyboard)
