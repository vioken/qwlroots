#include "qwdamagering.h"
#include <QRect>
extern "C" {
#include <wlr/types/wlr_damage_ring.h>
#include <wlr/util/box.h>
QW_FUNC_MEMBER(damage, ring_init)
QW_FUNC_MEMBER(damage, ring_finish)
QW_FUNC_MEMBER(damage, ring_add)
QW_FUNC_MEMBER(damage, ring_add_box)
QW_FUNC_MEMBER(damage, ring_add_whole)
QW_FUNC_MEMBER(damage, ring_rotate)
QW_FUNC_MEMBER(damage, ring_get_buffer_damage)
QW_FUNC_MEMBER(damage, ring_set_bounds)
