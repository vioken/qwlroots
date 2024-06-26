#include "qwoutputinterface.h"
#include "qwbackend.h"
#include "types/qwbuffer.h"
#include "qwdisplay.h"
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
#include <wlr/interfaces/wlr_output.h>
QW_INTERFACE(set_cursor)
QW_INTERFACE(move_cursor)
QW_INTERFACE(destroy)
QW_INTERFACE(test)
QW_INTERFACE(commit)
QW_INTERFACE(get_gamma_size)
QW_INTERFACE(get_cursor_formats)
QW_INTERFACE(get_cursor_size)
QW_INTERFACE(get_primary_formats)
#if WLR_VERSION_MINOR > 17
#else
#endif
