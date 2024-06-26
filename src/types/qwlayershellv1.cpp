#include "qwlayershellv1.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "qwxdgshell.h"
#include "private/qwglobal_p.h"
#include <QHash>
#include <QPointF>
extern "C" {
#include <math.h>
#define namespace scope
#define static
#include <wlr/types/wlr_layer_shell_v1.h>
#undef namespace
#undef static
QW_SIGNAL(new_surface, arguments?)
QW_SIGNAL(new_popup, arguments?)
