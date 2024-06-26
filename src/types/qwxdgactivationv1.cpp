#include "qwxdgactivationv1.h"
#include "private/qwglobal_p.h"
#include <qwdisplay.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_xdg_activation_v1.h>
QW_SIGNAL(request_activate, arguments?)
QW_SIGNAL(new_token, arguments?)
