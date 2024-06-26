#include "qwtexture.h"
#include "qwrenderer.h"
#include "types/qwbuffer.h"
#include <QHash>
extern "C" {
#include <wlr/render/wlr_texture.h>
QW_FUNC_MEMBER(texture, destroy)
QW_FUNC_MEMBER(texture, from_pixels)
QW_FUNC_MEMBER(texture, from_dmabuf)
QW_FUNC_MEMBER(texture, from_buffer)
QW_FUNC_MEMBER(texture, update_from_buffer)
