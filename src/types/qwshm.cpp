#include "qwshm.h"
#include "qwrenderer.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_shm.h>
QW_FUNC_MEMBER(shm, create)
QW_FUNC_MEMBER(shm, create_with_renderer)
