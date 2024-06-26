#include "qwinputinhibitmanager.h"
#include "private/qwglobal_p.h"
#include <qwdisplay.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_input_inhibitor.h>
QW_SIGNAL(activate, arguments?)
QW_SIGNAL(deactivate, arguments?)
QW_FUNC_MEMBER(input, inhibit_manager_create)
