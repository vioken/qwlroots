#include "qwinputmethodv2.h"
#include "qwkeyboard.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
