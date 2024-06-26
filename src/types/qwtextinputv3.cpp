#include "qwtextinputv3.h"
#include "private/qwglobal_p.h"
#include <qwcompositor.h>
#include <QHash>
extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
QW_SIGNAL(enable, arguments?)
QW_SIGNAL(commit, arguments?)
QW_SIGNAL(disable, arguments?)
