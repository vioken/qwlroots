// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtextinputv3.h"
#include "private/qwglobal_p.h"

#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
}

QW_BEGIN_NAMESPACE

class QWTextInputV3Private : public QWWrapObjectPrivate
{
public:
    QWTextInputV3Private(wlr_text_input_v3 *handle, bool isOwner, QWTextInputV3 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.enable, this, &QWTextInputV3Private::on_enable);
        sc.connect(&handle->events.commit, this, &QWTextInputV3Private::on_commit);
        sc.connect(&handle->events.disable, this, &QWTextInputV3Private::on_disable);
    }

    void on_enable(wlr_text_input_v3 *);
    void on_commit(wlr_text_input_v3 *);
    void on_disable(wlr_text_input_v3 *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWTextInputV3)
};
QHash<void*, QWWrapObject*> QWTextInputV3Private::map;

void QWTextInputV3Private::on_enable(wlr_text_input_v3 *handle)
{
    Q_EMIT q_func()->enable(QWTextInputV3::from(handle));
}

void QWTextInputV3Private::on_commit(wlr_text_input_v3 *handle)
{
    Q_EMIT q_func()->commit(QWTextInputV3::from(handle));
}

void QWTextInputV3Private::on_disable(wlr_text_input_v3 *handle)
{
    Q_EMIT q_func()->disable(QWTextInputV3::from(handle));
}

QWTextInputV3::QWTextInputV3(wlr_text_input_v3 *handle, bool isOwner)
    : QWWrapObject(*new QWTextInputV3Private(handle, isOwner, this))
{

}

QWTextInputV3 *QWTextInputV3::get(wlr_text_input_v3 *handle)
{
    return static_cast<QWTextInputV3*>(QWTextInputV3Private::map.value(handle));
}

QWTextInputV3 *QWTextInputV3::from(wlr_text_input_v3 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTextInputV3(handle, false);
}

void QWTextInputV3::sendEnter(QWSurface *wlr_surface)
{
    wlr_text_input_v3_send_enter(handle(), wlr_surface->handle());
}

void QWTextInputV3::sendLeave()
{
    wlr_text_input_v3_send_leave(handle());
}

void QWTextInputV3::sendPreeditString(const char *text, int32_t cursor_begin, int32_t cursor_end)
{
    wlr_text_input_v3_send_preedit_string(handle(), text, cursor_begin, cursor_end);
}

void QWTextInputV3::sendCommitString(const char *text)
{
    wlr_text_input_v3_send_commit_string(handle(), text);
}

void QWTextInputV3::sendDeleteSurroundingText(uint32_t before_length, uint32_t after_length)
{
    wlr_text_input_v3_send_delete_surrounding_text(handle(), before_length, after_length);
}

void QWTextInputV3::sendDone()
{
    wlr_text_input_v3_send_done(handle());
}

QW_END_NAMESPACE
