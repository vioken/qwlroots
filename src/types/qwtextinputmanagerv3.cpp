// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtextinputv3.h"
#include "util/qwsignalconnector.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
}

QW_BEGIN_NAMESPACE

class QWTextInputManagerV3Private : public QWObjectPrivate
{
public:
    QWTextInputManagerV3Private(wlr_text_input_manager_v3 *handle, bool isOwner, QWTextInputManagerV3 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWTextInputManagerV3Private::on_destroy);
        sc.connect(&handle->events.text_input, this, &QWTextInputManagerV3Private::on_text_input);
    }
    ~QWTextInputManagerV3Private() {
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
    void on_text_input(wlr_text_input_v3 *);

    static QHash<void*, QWTextInputManagerV3*> map;
    QW_DECLARE_PUBLIC(QWTextInputManagerV3)
    QWSignalConnector sc;
};
QHash<void*, QWTextInputManagerV3*> QWTextInputManagerV3Private::map;

void QWTextInputManagerV3Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWTextInputManagerV3Private::on_text_input(wlr_text_input_v3 *input)
{
    Q_EMIT q_func()->textInput(QWTextInputV3::from(input));
}

QWTextInputManagerV3::QWTextInputManagerV3(wlr_text_input_manager_v3 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWTextInputManagerV3Private(handle, isOwner, this))
{

}

QWTextInputManagerV3 *QWTextInputManagerV3::get(wlr_text_input_manager_v3 *handle)
{
    return QWTextInputManagerV3Private::map.value(handle);
}

QWTextInputManagerV3 *QWTextInputManagerV3::from(wlr_text_input_manager_v3 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWTextInputManagerV3(handle, false);
}

QWTextInputManagerV3 *QWTextInputManagerV3::create(QWDisplay *wl_display)
{
    auto *handle = wlr_text_input_manager_v3_create(wl_display->handle());
    return handle ? new QWTextInputManagerV3(handle, true) : nullptr;
}

QW_END_NAMESPACE
