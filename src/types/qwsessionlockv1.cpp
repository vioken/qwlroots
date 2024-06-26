#include "qwsessionlockv1.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_session_lock_v1.h>
QW_SIGNAL(new_surface, arguments?)
QW_SIGNAL(unlock, arguments?)
