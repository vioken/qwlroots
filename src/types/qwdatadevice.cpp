#include "qwdatadevice.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_data_device.h>
QW_FUNC_MEMBER(data, device_manager_create)
