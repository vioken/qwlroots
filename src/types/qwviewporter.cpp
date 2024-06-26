#include "qwviewporter.h"
#include "private/qwglobal_p.h"
#include "qwdisplay.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_viewporter.h>
QW_FUNC_MEMBER(viewporter, create)
