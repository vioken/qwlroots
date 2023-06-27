// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_linux_dmabuf_v1;
struct wlr_linux_dmabuf_feedback_v1;
struct wlr_linux_dmabuf_feedback_v1_init_options;
struct wlr_dmabuf_v1_buffer;

QW_BEGIN_NAMESPACE

class QWDisplay;
class QWRenderer;
class QW_EXPORT QWLinuxDmabufFeedbackV1
{
public:
    QWLinuxDmabufFeedbackV1() = delete;
    ~QWLinuxDmabufFeedbackV1() = delete;

    wlr_linux_dmabuf_feedback_v1 *handle() const;
    static QWLinuxDmabufFeedbackV1 *from(wlr_linux_dmabuf_feedback_v1 *handle);

#if WLR_VERSION_MINOR > 16
    void finish();
    bool initWithOptions(const wlr_linux_dmabuf_feedback_v1_init_options *options);
#endif // WLR_VERSION_MINOR > 16
};

class QWLinuxDmabufV1Private;
class QW_EXPORT QWLinuxDmabufV1 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWLinuxDmabufV1)
public:
    inline wlr_linux_dmabuf_v1 *handle() const {
        return QWObject::handle<wlr_linux_dmabuf_v1>();
    }

    static QWLinuxDmabufV1 *get(wlr_linux_dmabuf_v1 *handle);
    static QWLinuxDmabufV1 *from(wlr_linux_dmabuf_v1 *handle);
#if WLR_VERSION_MINOR > 16
    static QWLinuxDmabufV1 *create(QWDisplay *display, uint32_t version, const QWLinuxDmabufFeedbackV1 *defaultFeedback);
    static QWLinuxDmabufV1 *create(QWDisplay *display, uint32_t version, QWRenderer *renderer);
#else
    static QWLinuxDmabufV1 *create(QWDisplay *display, QWRenderer *renderer);
#endif // WLR_VERSION_MINOR > 16

Q_SIGNALS:
    void beforeDestroy(QWLinuxDmabufV1 *self);

private:
    QWLinuxDmabufV1(wlr_linux_dmabuf_v1 *handle, bool isOwner);
    ~QWLinuxDmabufV1() = default;
};

QW_END_NAMESPACE
