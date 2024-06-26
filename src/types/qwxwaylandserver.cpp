#include "qwxwaylandserver.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/xwayland/server.h>
QW_SIGNAL(start, arguments?)
QW_SIGNAL(ready, arguments?)
QW_FUNC_MEMBER(xwayland, server_create)
