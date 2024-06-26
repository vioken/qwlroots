#include "qwidleinhibitv1.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_idle_inhibit_v1.h>
QW_SIGNAL(new_inhibitor, arguments?)
