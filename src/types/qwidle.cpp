#include "qwidle.h"
#include "qwseat.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_idle.h>
QW_SIGNAL(activity_notify, arguments?)
QW_FUNC_MEMBER(idle, create)
QW_FUNC_MEMBER(idle, notify_activity)
QW_FUNC_MEMBER(idle, set_enabled)
QW_SIGNAL(idle, arguments?)
QW_SIGNAL(resume, arguments?)
QW_FUNC_MEMBER(idle, timeout_create)
