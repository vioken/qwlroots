#include "qwinputdevice.h"
#include "qwinputdevice_p.h"
#include "qwkeyboard.h"
#include "qwpointer.h"
#include "qwtablet.h"
#include "qwtabletpad.h"
#include "qwswitch.h"
#include "qwtouch.h"
extern "C" {
#include <wlr/types/wlr_input_device.h>
#if WLR_VERSION_MINOR > 17
#else
#endif
