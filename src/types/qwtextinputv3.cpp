// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtextinputv3.h"
#include "util/qwsignalconnector.h"

#include <qwcompositor.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
}

QW_BEGIN_NAMESPACE

class QWTextInputV3Private : public QWObjectPrivate
{
public:
    QWTextInputV3Private(wlr_text_input_v3 *handle, bool isOwner, QWTextInputV3 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWTextInputV3Private::on_destroy);
        sc.connect(&handle->events.enable, this, &QWTextInputV3Private::on_enable);
        sc.connect(&handle->events.commit, this, &QWTextInputV3Private::on_commit);
        sc.connect(&handle->events.disable, this, &QWTextInputV3Private::on_disable);
    }
    ~QWTextInputV3Private() {
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
    void on_enable(wlr_text_input_v3 *);
    void on_commit(wlr_text_input_v3 *);
    void on_disable(wlr_text_input_v3 *);

    static QHash<void*, QWTextInputV3*> map;
    QW_DECLARE_PUBLIC(QWTextInputV3)
    QWSignalConnector sc;
};
QHash<void*, QWTextInputV3*> QWTextInputV3Private::map;

void QWTextInputV3Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

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
    : QObject(nullptr)
    , QWObject(*new QWTextInputV3Private(handle, isOwner, this))
{

}

QWTextInputV3 *QWTextInputV3::get(wlr_text_input_v3 *handle)
{
    return QWTextInputV3Private::map.value(handle);
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
