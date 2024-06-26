#include "qwbufferinterface.h"
#include <qwbuffer.h>
extern "C" {
#include <wlr/interfaces/wlr_buffer.h>
#include <wlr/types/wlr_buffer.h>
QW_INTERFACE(destroy)
QW_INTERFACE(get_dmabuf)
QW_INTERFACE(get_shm)
QW_INTERFACE(begin_data_ptr_access)
QW_INTERFACE(end_data_ptr_access)
