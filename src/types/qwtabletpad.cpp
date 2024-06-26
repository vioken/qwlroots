#include "qwtabletpad.h"
#include "qwinputdevice_p.h"
#include "private/qwglobal_p.h"
extern "C" {
#include <wlr/types/wlr_tablet_pad.h>
#include <wlr/types/wlr_tablet_tool.h>
QW_SIGNAL(button, arguments?)
QW_SIGNAL(ring, arguments?)
QW_SIGNAL(strip, arguments?)
QW_SIGNAL(attach_tablet, arguments?)
QW_FUNC_MEMBER(tablet, pad_from_input_device)
