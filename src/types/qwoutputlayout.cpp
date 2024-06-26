#include "qwoutputlayout.h"
#include "private/qwglobal_p.h"
#include "qwoutput.h"
#include "qwdisplay.h"
#include <QPointF>
#include <QRect>
#include <QHash>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output_layout.h>
#undef static
#include <wlr/util/box.h>
QW_SIGNAL(add, arguments?)
QW_SIGNAL(change, arguments?)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, layout_create)
#else
QW_FUNC_MEMBER(output, layout_create)
#endif
QW_FUNC_MEMBER(output, layout_get)
QW_FUNC_MEMBER(output, layout_output_at)
QW_FUNC_MEMBER(output, layout_get_center_output)
QW_FUNC_MEMBER(output, layout_adjacent_output)
QW_FUNC_MEMBER(output, layout_farthest_output)
QW_FUNC_MEMBER(output, layout_add)
QW_FUNC_MEMBER(output, layout_add_auto)
QW_FUNC_MEMBER(output, layout_add)
QW_FUNC_MEMBER(output, layout_remove)
QW_FUNC_MEMBER(output, layout_output_coords)
QW_FUNC_MEMBER(output, layout_contains_point)
QW_FUNC_MEMBER(output, layout_intersects)
QW_FUNC_MEMBER(output, layout_closest_point)
QW_FUNC_MEMBER(output, layout_get_box)
