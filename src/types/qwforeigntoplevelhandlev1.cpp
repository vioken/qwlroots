#include "qwforeigntoplevelhandlev1.h"
#include "qwoutput.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include "math.h"
#define static
#include <wlr/types/wlr_foreign_toplevel_management_v1.h>
#undef static
QW_SIGNAL(request_maximize, arguments?)
QW_SIGNAL(request_minimize, arguments?)
QW_SIGNAL(request_activate, arguments?)
QW_SIGNAL(request_fullscreen, arguments?)
QW_SIGNAL(request_close, arguments?)
QW_SIGNAL(set_rectangle, arguments?)
