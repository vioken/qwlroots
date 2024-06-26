#include "qwxdgdecorationmanagerv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_decoration_v1.h>
#undef static
QW_SIGNAL(new_toplevel_decoration, arguments?)
QW_SIGNAL(request_mode, arguments?)
