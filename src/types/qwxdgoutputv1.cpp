#include "qwxdgoutputv1.h"
#include "qwdisplay.h"
#include "qwoutputlayout.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_output_v1.h>
#undef static
