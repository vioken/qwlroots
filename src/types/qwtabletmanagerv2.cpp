#include "qwtabletv2.h"
#include "qwtablet.h"
#include "private/qwglobal_p.h"
#include <qwinputdevice.h>
#include <qwseat.h>
#include <qwdisplay.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_tablet_v2.h>
QW_FUNC_MEMBER(tablet, create)
QW_FUNC_MEMBER(tablet, pad_create)
QW_FUNC_MEMBER(tablet, tool_create)
