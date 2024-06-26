#include "qwxdgforeignregistry.h"
#include "private/qwglobal_p.h"
#include <qwglobal.h>
#include <qwxdgforeignregistry_p.h>
#include <qwdisplay.h>
extern "C" {
#include <wlr/types/wlr_xdg_foreign_registry.h>
QW_FUNC_MEMBER(xdg, foreign_registry_create)
QW_FUNC_MEMBER(xdg, foreign_registry_find_by_handle)
QW_FUNC_MEMBER(xdg, foreign_exported_init)
QW_FUNC_MEMBER(xdg, foreign_exported_finish)
