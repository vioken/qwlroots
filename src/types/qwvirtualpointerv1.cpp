#include "qwvirtualpointerv1.h"
#include "private/qwglobal_p.h"
#include <qwdisplay.h>
#include <QHash>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_virtual_pointer_v1.h>
#undef static
QW_SIGNAL(new_virtual_pointer, arguments?)
