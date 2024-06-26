#include "qwinputmethodv2.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
QW_SIGNAL(commit, arguments?)
QW_SIGNAL(new_popup_surface, arguments?)
QW_SIGNAL(grab_keyboard, arguments?)
