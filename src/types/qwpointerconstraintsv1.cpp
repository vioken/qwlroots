#include "qwpointerconstraintsv1.h"
#include "private/qwglobal_p.h"
#include <qwdisplay.h>
#include <qwseat.h>
#include <qwcompositor.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_pointer_constraints_v1.h>
QW_SIGNAL(new_constraint, arguments?)
