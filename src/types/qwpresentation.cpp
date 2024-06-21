// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpresentation.h"
#include "qwdisplay.h"
#include "qwoutput.h"
#include "qwcompositor.h"
#include "qw_backend.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_presentation_time.h>
}

QW_BEGIN_NAMESPACE

class QWPresentationPrivate : public QWWrapObjectPrivate
{
public:
    QWPresentationPrivate(wlr_presentation *handle, bool isOwner, QWPresentation *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {

    }

    QW_DECLARE_PUBLIC(QWPresentation)
};

QWPresentation::QWPresentation(wlr_presentation *handle, bool isOwner)
    : QWWrapObject(*new QWPresentationPrivate(handle, isOwner, this))
{

}

QWPresentation *QWPresentation::create(QWDisplay *display, QWBackend *backend)
{
    auto handle = wlr_presentation_create(display->handle(), backend->handle());
    if (!handle)
        return nullptr;
    return new QWPresentation(handle, true);
}

QWPresentation *QWPresentation::get(wlr_presentation *handle)
{
    return static_cast<QWPresentation*>(QWPresentationPrivate::map.value(handle));
}

QWPresentation *QWPresentation::from(wlr_presentation *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPresentation(handle, false);
}

QWPresentationFeedback *QWPresentation::surfaceSampled(QWSurface *surface) const
{
#if WLR_VERSION_MINOR > 17
    return QWPresentationFeedback::from(wlr_presentation_surface_sampled(surface->handle()));
#else
    return QWPresentationFeedback::from(wlr_presentation_surface_sampled(handle(), surface->handle()));
#endif
}

void QWPresentation::surfaceTexturedOnOutput(QWSurface *surface, QWOutput *output)
{
#if WLR_VERSION_MINOR > 17
    wlr_presentation_surface_textured_on_output(surface->handle(), output->handle());
#else
    wlr_presentation_surface_textured_on_output(handle(), surface->handle(), output->handle());
#endif
}

void QWPresentation::surfaceScannedOutOnOutput(QWSurface *surface, QWOutput *output)
{
#if WLR_VERSION_MINOR > 17
    wlr_presentation_surface_scanned_out_on_output(surface->handle(), output->handle());
#else
    wlr_presentation_surface_scanned_out_on_output(handle(), surface->handle(), output->handle());
#endif
}

wlr_presentation_event *QWPresentationEvent::handle() const
{
    return reinterpret_cast<wlr_presentation_event*>(const_cast<QWPresentationEvent*>(this));
}

QWPresentationEvent *QWPresentationEvent::from(wlr_presentation_event* event)
{
    return reinterpret_cast<QWPresentationEvent*>(event);
}

void QWPresentationEvent::fromOutput(wlr_presentation_event *handle, const wlr_output_event_present *outputEvent)
{
    wlr_presentation_event_from_output(handle, outputEvent);
}

void QWPresentationFeedback::operator delete(QWPresentationFeedback *p, std::destroying_delete_t)
{
    wlr_presentation_feedback_destroy(p->handle());
}

wlr_presentation_feedback *QWPresentationFeedback::handle() const
{
    return reinterpret_cast<wlr_presentation_feedback*>(const_cast<QWPresentationFeedback*>(this));
}

QWPresentationFeedback *QWPresentationFeedback::from(wlr_presentation_feedback *handle)
{
    return reinterpret_cast<QWPresentationFeedback*>(handle);
}

void QWPresentationFeedback::sendPresented(QWPresentationEvent *event)
{
    return wlr_presentation_feedback_send_presented(handle(), event->handle());
}

QW_END_NAMESPACE
