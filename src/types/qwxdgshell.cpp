#include "qwxdgshell.h"
#include "qwdisplay.h"
#include "qwcompositor.h"
#include "private/qwglobal_p.h"
#include <QPointF>
#include <QRect>
#include <QHash>
extern "C" {
#include <math.h>
#define static
#include <wlr/types/wlr_xdg_shell.h>
#undef static
QW_SIGNAL(new_surface, arguments?)
#if WLR_VERSION_MINOR >= 18
QW_SIGNAL(new_toplevel, arguments?)
QW_SIGNAL(new_popup, arguments?)
#endif
#if WLR_VERSION_MINOR >= 18
#endif
#if WLR_VERSION_MINOR >= 18
#endif
QW_FUNC_MEMBER(xdg, shell_create)
QW_SIGNAL(ping_timeout, arguments?)
QW_SIGNAL(new_popup, arguments?)
QW_SIGNAL(configure, arguments?)
QW_SIGNAL(ack_configure, arguments?)
#if WLR_VERSION_MINOR >= 18
#endif
QW_FUNC_MEMBER(xdg, surface_from_resource)
QW_FUNC_MEMBER(xdg, surface_try_from_wlr_surface)
QW_FUNC_MEMBER(xdg, surface_ping)
QW_FUNC_MEMBER(xdg, surface_schedule_configure)
QW_FUNC_MEMBER(xdg, surface_surface_at)
QW_FUNC_MEMBER(xdg, surface_popup_surface_at)
QW_FUNC_MEMBER(xdg, surface_get_geometry)
QW_FUNC_MEMBER(xdg, surface_for_each_surface)
QW_FUNC_MEMBER(xdg, surface_for_each_popup_surface)
#if WLR_VERSION_MINOR >= 18
#else
#endif
QW_SIGNAL(reposition, arguments?)
QW_FUNC_MEMBER(xdg, popup_from_resource)
QW_FUNC_MEMBER(xdg, popup_get_position)
QW_FUNC_MEMBER(xdg, popup_get_toplevel_coords)
QW_FUNC_MEMBER(xdg, popup_unconstrain_from_box)
#if WLR_VERSION_MINOR >= 18
#else
#endif
QW_SIGNAL(request_maximize, arguments?)
QW_SIGNAL(request_fullscreen, arguments?)
QW_SIGNAL(request_minimize, arguments?)
QW_SIGNAL(request_move, arguments?)
QW_SIGNAL(request_resize, arguments?)
QW_SIGNAL(request_show_window_menu, arguments?)
QW_SIGNAL(set_parent, arguments?)
QW_SIGNAL(set_title, arguments?)
QW_SIGNAL(set_app_id, arguments?)
QW_FUNC_MEMBER(xdg, toplevel_from_resource)
QW_FUNC_MEMBER(xdg, toplevel_set_size)
QW_FUNC_MEMBER(xdg, toplevel_set_activated)
QW_FUNC_MEMBER(xdg, toplevel_set_maximized)
QW_FUNC_MEMBER(xdg, toplevel_set_fullscreen)
QW_FUNC_MEMBER(xdg, toplevel_set_resizing)
QW_FUNC_MEMBER(xdg, toplevel_set_tiled)
QW_FUNC_MEMBER(xdg, toplevel_set_bounds)
QW_FUNC_MEMBER(xdg, toplevel_set_wm_capabilities)
QW_FUNC_MEMBER(xdg, toplevel_send_close)
QW_FUNC_MEMBER(xdg, toplevel_set_parent)
