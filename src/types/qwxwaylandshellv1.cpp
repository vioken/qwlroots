#include "qwxwaylandshellv1.h"
#include "qwglobal.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include <private/qwglobal_p.h>
#include <QHash>
extern "C" {
#include <wlr/xwayland/shell.h>
QW_SIGNAL(new_surface, arguments?)
