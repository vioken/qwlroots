#include "qwtablet.h"
#include "qwinputdevice_p.h"
#include "private/qwglobal_p.h"
extern "C" {
#include <wlr/types/wlr_tablet_tool.h>
QW_SIGNAL(axis, arguments?)
QW_SIGNAL(proximity, arguments?)
QW_SIGNAL(tip, arguments?)
QW_SIGNAL(button, arguments?)
QW_FUNC_MEMBER(tablet, from_input_device)
