#include "qwinputdevice.h"
#include "qwtabletv2.h"
#include "private/qwglobal_p.h"
#include <qwcompositor.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
QW_SIGNAL(button_feedback, arguments?)
QW_SIGNAL(ring_feedback, arguments?)
QW_SIGNAL(strip_feedback, arguments?)
