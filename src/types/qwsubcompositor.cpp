#include "qwsubcompositor.h"
#include "qwcompositor.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_subcompositor.h>
QW_FUNC_MEMBER(subcompositor, create)
QW_FUNC_MEMBER(subsurface, try_from_wlr_surface)
