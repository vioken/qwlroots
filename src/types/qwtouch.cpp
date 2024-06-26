#include "qwtouch.h"
#include "qwinputdevice_p.h"
#include "private/qwglobal_p.h"
extern "C" {
#include <wlr/types/wlr_touch.h>
QW_SIGNAL(down, arguments?)
QW_SIGNAL(up, arguments?)
QW_SIGNAL(motion, arguments?)
QW_SIGNAL(cancel, arguments?)
QW_SIGNAL(frame, arguments?)
QW_FUNC_MEMBER(touch, from_input_device)
