#include "qwallocator.h"
#include "qwbackend.h"
#include "qwrenderer.h"
#include "qwbuffer.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/render/allocator.h>
QW_FUNC_MEMBER(allocator, autocreate)
QW_FUNC_MEMBER(allocator, create_buffer)
