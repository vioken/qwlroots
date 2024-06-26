#include "qwdrm.h"
#include "qwdisplay.h"
#include "qwrenderer.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wayland-server.h>
#include <wlr/types/wlr_drm.h>
QW_FUNC_MEMBER(drm, create)
QW_FUNC_MEMBER(drm, buffer_try_from_resource)
