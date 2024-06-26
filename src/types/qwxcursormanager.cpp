#include "qwxcursormanager.h"
#include "qwcursor.h"
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xcursor_manager.h>
#undef static
QW_FUNC_MEMBER(xcursor, manager_destroy)
QW_FUNC_MEMBER(xcursor, manager_create)
QW_FUNC_MEMBER(xcursor, manager_load)
QW_FUNC_MEMBER(xcursor, manager_get_xcursor)
