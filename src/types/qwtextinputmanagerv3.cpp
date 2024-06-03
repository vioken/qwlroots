// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwtextinputv3.h"
#include "private/qwglobal_p.h"

#include <qwdisplay.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_text_input_v3.h>
}

QW_BEGIN_NAMESPACE

class QWTextInputManagerV3Private : public QWWrapObjectPrivate
{
public:
    QWTextInputManagerV3Private(wlr_text_input_manager_v3 *handle, bool isOwner, QWTextInputManagerV3 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &handle->events.destroy)
    {
        sc.connect(&handle->events.text_input, this, &QWTextInputManagerV3Private::on_text_input);
    }

    void on_text_input(wlr_text_input_v3 *);

    QW_DECLARE_PUBLIC(QWTextInputManagerV3)
};

void QWTextInputManagerV3Private::on_text_input(wlr_text_input_v3 *input)
{
    Q_EMIT q_func()->textInput(QWTextInputV3::from(input));
}

QWTextInputManagerV3::QWTextInputManagerV3(wlr_text_input_manager_v3 *handle, bool isOwner)
    : QWWrapObject(*new QWTextInputManagerV3Private(handle, isOwner, this))
{

}

QWTextInputManagerV3 *QWTextInputManagerV3::get(wlr_text_input_manager_v3 *handle)
{
    return static_cast<QWTextInputManagerV3*>(QWTextInputManagerV3Private::map.value(handle));
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
