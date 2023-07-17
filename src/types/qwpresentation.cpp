// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwpresentation.h"
#include "qwdisplay.h"
#include "qwoutput.h"
#include "qwcompositor.h"
#include "qwbackend.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_presentation_time.h>
}

QW_BEGIN_NAMESPACE

class QWPresentationPrivate : public QWObjectPrivate
{
public:
    QWPresentationPrivate(wlr_presentation *handle, bool isOwner, QWPresentation *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWPresentationPrivate::on_destroy);
    }
    ~QWPresentationPrivate() {
        if (!m_handle)
            return;
        destroy();
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWPresentation*> map;
    QW_DECLARE_PUBLIC(QWPresentation)
    QWSignalConnector sc;
};
QHash<void*, QWPresentation*> QWPresentationPrivate::map;

void QWPresentationPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWPresentation::QWPresentation(wlr_presentation *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWPresentationPrivate(handle, isOwner, this))
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
    return QWPresentationPrivate::map.value(handle);
}

QWPresentation *QWPresentation::from(wlr_presentation *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPresentation(handle, false);
}

QWPresentationFeedback *QWPresentation::surfaceSampled(QWSurface *surface) const
{
    return QWPresentationFeedback::from(wlr_presentation_surface_sampled(handle(), surface->handle()));
}

#if WLR_VERSION_MINOR > 16
void QWPresentation::surfaceTexturedOnOutput(QWSurface *surface, QWOutput *output)
{
    wlr_presentation_surface_textured_on_output(handle(), surface->handle(), output->handle());
}

void QWPresentation::surfaceScannedOutOnOutput(QWSurface *surface, QWOutput *output)
{
    wlr_presentation_surface_scanned_out_on_output(handle(), surface->handle(), output->handle());
}

#else

void QWPresentation::surfaceSampledOnOutput(QWSurface *surface, QWOutput *output)
{
    wlr_presentation_surface_sampled_on_output(handle(), surface->handle(), output->handle());
}
#endif

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
