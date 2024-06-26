#include "qwpresentation.h"
#include "qwdisplay.h"
#include "qwoutput.h"
#include "qwcompositor.h"
#include "qwbackend.h"
#include "private/qwglobal_p.h"
#include <QHash>
extern "C" {
#include <wlr/types/wlr_presentation_time.h>
QW_FUNC_MEMBER(presentation, create)
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(presentation, surface_sampled)
#else
QW_FUNC_MEMBER(presentation, surface_sampled)
#endif
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(presentation, surface_textured_on_output)
#else
QW_FUNC_MEMBER(presentation, surface_textured_on_output)
#endif
#if WLR_VERSION_MINOR > 17
QW_FUNC_MEMBER(presentation, surface_scanned_out_on_output)
#else
QW_FUNC_MEMBER(presentation, surface_scanned_out_on_output)
#endif
QW_FUNC_MEMBER(presentation, event_from_output)
QW_FUNC_MEMBER(presentation, feedback_destroy)
QW_FUNC_MEMBER(presentation, feedback_send_presented)
