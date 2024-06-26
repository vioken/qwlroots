#include "qwxwayland.h"
#include "qwxwaylandserver.h"
#include "private/qwglobal_p.h"
#include <qwdisplay.h>
#include <qwcompositor.h>
#include <qwseat.h>
#include <qwxwaylandshellv1.h>
#include <QImage>
#include <QPoint>
#include <QHash>
extern "C" {
#include <math.h>
#define class _class
#include <wlr/xwayland/xwayland.h>
#undef class
QW_SIGNAL(ready, arguments?)
QW_SIGNAL(new_surface, arguments?)
QW_SIGNAL(remove_startup_info, arguments?)
QW_FUNC_MEMBER(xwayland, create)
QW_FUNC_MEMBER(xwayland, set_cursor)
QW_FUNC_MEMBER(xwayland, set_seat)
