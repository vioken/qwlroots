#include "qwbuffer.h"
#include "render/qwrenderer.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_buffer.h>
QW_SIGNAL(release, arguments?)
QW_FUNC_MEMBER(buffer, try_from_resource)
QW_FUNC_MEMBER(buffer, drop)
QW_FUNC_MEMBER(buffer, lock)
QW_FUNC_MEMBER(buffer, unlock)
QW_FUNC_MEMBER(buffer, get_dmabuf)
QW_FUNC_MEMBER(buffer, get_shm)
QW_FUNC_MEMBER(buffer, begin_data_ptr_access)
QW_FUNC_MEMBER(buffer, end_data_ptr_access)
