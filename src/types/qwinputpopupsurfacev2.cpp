#include "qwinputmethodv2.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"
#include <QHash>
#include <QRect>
extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
#include <wlr/util/box.h>
