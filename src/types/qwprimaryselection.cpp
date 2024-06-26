#include "qwprimaryselection.h"
#include "private/qwglobal_p.h"
#include <qwseat.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_primary_selection.h>
QW_FUNC_MEMBER(primary, selection_source_init)
QW_FUNC_MEMBER(primary, selection_source_send)
QW_FUNC_MEMBER(seat, request_set_primary_selection)
QW_FUNC_MEMBER(seat, set_primary_selection)
