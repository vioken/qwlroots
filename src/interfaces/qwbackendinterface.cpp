#include "qwbackendinterface.h"
extern "C" {
#include <wlr/backend/interface.h>
QW_INTERFACE(start)
QW_INTERFACE(destroy)
QW_INTERFACE(get_drm_fd)
QW_INTERFACE(get_buffer_caps)
