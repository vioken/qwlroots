// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_presentation;
struct wlr_presentation_feedback;
struct wlr_presentation_event;
struct wlr_output_event_present;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWOutput;
class QWDisplay;
class QWBackend;
class QWPresentationFeedback;
class QWPresentationPrivate;
class QW_EXPORT QWPresentation : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPresentation)
public:
    inline wlr_presentation *handle() const {
        return QWObject::handle<wlr_presentation>();
    }

    static QWPresentation *create(QWDisplay *display, QWBackend *backend);
    static QWPresentation *get(wlr_presentation *handle);
    static QWPresentation *from(wlr_presentation *handle);

    QWPresentationFeedback *surfaceSampled(QWSurface *surface) const;

#if WLR_VERSION_MINOR > 16
    void surfaceTexturedOnOutput(QWSurface *surface, QWOutput *output);
    void surfaceScannedOutOnOutput(QWSurface *surface, QWOutput *output);
#else
    void surfaceSampledOnOutput(QWSurface *surface, QWOutput *output);
#endif

Q_SIGNALS:
    void beforeDestroy(QWPresentation *self);

private:
    QWPresentation(wlr_presentation *handle, bool isOwner);
    ~QWPresentation() = default;
};

class QW_EXPORT QWPresentationEvent {
public:
    QWPresentationEvent() = delete;
    ~QWPresentationEvent() = delete;

    wlr_presentation_event *handle() const;

    static QWPresentationEvent* from(wlr_presentation_event *event);
    static void fromOutput(wlr_presentation_event *handle, const wlr_output_event_present *outputEvent);
};

class QW_EXPORT QWPresentationFeedback {
public:
    QWPresentationFeedback() = delete;
    ~QWPresentationFeedback() = delete;

    void operator delete(QWPresentationFeedback *p, std::destroying_delete_t);

    wlr_presentation_feedback *handle() const;

    static QWPresentationFeedback* from(wlr_presentation_feedback *handle);

    void sendPresented(QWPresentationEvent *event);
};

QW_END_NAMESPACE
