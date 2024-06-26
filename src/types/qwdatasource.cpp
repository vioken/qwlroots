#include "qwdatadevice.h"
extern "C" {
#include <wlr/types/wlr_data_device.h>
QW_FUNC_MEMBER(data, source_destroy)
QW_FUNC_MEMBER(data, source_accept)
QW_FUNC_MEMBER(data, source_dnd_action)
QW_FUNC_MEMBER(data, source_dnd_drop)
QW_FUNC_MEMBER(data, source_dnd_finish)
QW_FUNC_MEMBER(data, source_send)
