// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_text_input_v3;
struct wlr_text_input_v3;
struct wlr_text_input_manager_v3;

QW_BEGIN_NAMESPACE

class QWSurface;
class QWDisplay;
class QWTextInputManagerV3;
class QWTextInputV3Private;
class QW_EXPORT QWTextInputV3 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTextInputV3)
public:
    inline wlr_text_input_v3 *handle() const {
        return QWObject::handle<wlr_text_input_v3>();
    }

    static QWTextInputV3 *get(wlr_text_input_v3 *handle);
    static QWTextInputV3 *from(wlr_text_input_v3 *handle);

    void sendEnter(QWSurface *wlr_surface);
    void sendLeave();
    void sendPreeditString(const char *text, int32_t cursor_begin, int32_t cursor_end);
    void sendCommitString(const char *text);
    void sendDeleteSurroundingText(uint32_t before_length, uint32_t after_length);
    void sendDone();

Q_SIGNALS:
    void beforeDestroy(QWTextInputV3 *self);
    void enable(QWTextInputV3 *);
    void commit(QWTextInputV3 *);
    void disable(QWTextInputV3 *);

private:
    QWTextInputV3(wlr_text_input_v3 *handle, bool isOwner);
    ~QWTextInputV3() = default;
};

class QWTextInputManagerV3Private;
class QW_EXPORT QWTextInputManagerV3 : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWTextInputManagerV3)
public:
    inline wlr_text_input_manager_v3 *handle() const {
        return QWObject::handle<wlr_text_input_manager_v3>();
    }

    static QWTextInputManagerV3 *get(wlr_text_input_manager_v3 *handle);
    static QWTextInputManagerV3 *from(wlr_text_input_manager_v3 *handle);
    static QWTextInputManagerV3 *create(QWDisplay *wl_display);

Q_SIGNALS:
    void beforeDestroy(QWTextInputManagerV3 *self);
    void textInput(QWTextInputV3 *);

private:
    QWTextInputManagerV3(wlr_text_input_manager_v3 *handle, bool isOwner);
    ~QWTextInputManagerV3() = default;
};

QW_END_NAMESPACE
