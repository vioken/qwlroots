#include "qwoutput.h"
#include "qwbuffer.h"
#include "qwdisplay.h"
#include "private/qwglobal_p.h"
#include "render/qwallocator.h"
#include "render/qwrenderer.h"
#include <QImage>
#include <QSize>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
QW_SIGNAL(frame, arguments?)
QW_SIGNAL(damage, arguments?)
QW_SIGNAL(needs_frame, arguments?)
QW_SIGNAL(precommit, arguments?)
QW_SIGNAL(commit, arguments?)
QW_SIGNAL(present, arguments?)
QW_SIGNAL(bind, arguments?)
QW_SIGNAL(description, arguments?)
QW_SIGNAL(request_state, arguments?)
QW_FUNC_MEMBER(output, from_resource)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, enable)
QW_FUNC_MEMBER(output, create_global)
#else
QW_FUNC_MEMBER(output, create_global)
#endif
QW_FUNC_MEMBER(output, destroy_global)
QW_FUNC_MEMBER(output, init_render)
QW_FUNC_MEMBER(output, preferred_mode)
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, set_mode)
QW_FUNC_MEMBER(output, set_custom_mode)
QW_FUNC_MEMBER(output, set_transform)
QW_FUNC_MEMBER(output, enable_adaptive_sync)
QW_FUNC_MEMBER(output, set_render_format)
QW_FUNC_MEMBER(output, set_scale)
QW_FUNC_MEMBER(output, set_subpixel)
QW_FUNC_MEMBER(output, set_damage)
QW_FUNC_MEMBER(output, set_gamma)
#endif
QW_FUNC_MEMBER(output, set_name)
QW_FUNC_MEMBER(output, set_description)
QW_FUNC_MEMBER(output, schedule_done)
QW_FUNC_MEMBER(output, transformed_resolution)
QW_FUNC_MEMBER(output, effective_resolution)
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, attach_render)
QW_FUNC_MEMBER(output, attach_buffer)
#endif
QW_FUNC_MEMBER(output, lock_attach_render)
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, preferred_read_format)
#endif
#if WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, test)
QW_FUNC_MEMBER(output, commit)
QW_FUNC_MEMBER(output, rollback)
#endif
QW_FUNC_MEMBER(output, test_state)
QW_FUNC_MEMBER(output, commit_state)
QW_FUNC_MEMBER(output, state_finish)
QW_FUNC_MEMBER(output, schedule_frame)
QW_FUNC_MEMBER(output, get_gamma_size)
QW_FUNC_MEMBER(output, lock_software_cursors)
#if WLR_VERSION_MAJOR == 0 && WLR_VERSION_MINOR < 18
QW_FUNC_MEMBER(output, render_software_cursors)
#endif
QW_FUNC_MEMBER(output, get_primary_formats)
QW_FUNC_MEMBER(output, add_software_cursors_to_render_pass)
QW_FUNC_MEMBER(output, configure_primary_swapchain)
QW_FUNC_MEMBER(output, cursor_destroy)
QW_FUNC_MEMBER(output, cursor_create)
QW_FUNC_MEMBER(output, cursor_set_buffer)
QW_FUNC_MEMBER(output, cursor_move)
