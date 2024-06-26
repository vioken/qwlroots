#include "qwswapchain.h"
#include "qwallocator.h"
#include "qwbuffer.h"
#include <QSize>
extern "C" {
#include <wlr/render/drm_format_set.h>
#include <wlr/render/swapchain.h>
QW_FUNC_MEMBER(swapchain, destroy)
QW_FUNC_MEMBER(swapchain, create)
QW_FUNC_MEMBER(swapchain, acquire)
QW_FUNC_MEMBER(swapchain, set_buffer_submitted)
