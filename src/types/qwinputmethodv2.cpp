// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "private/qwglobal_p.h"

#include <QHash>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
}

QW_BEGIN_NAMESPACE

class QWInputMethodV2Private : public QWWrapObjectPrivate
{
public:
    QWInputMethodV2Private(wlr_input_method_v2 *handle, bool isOwner, QWInputMethodV2 *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy)
    {
        sc.connect(&handle->events.commit, this, &QWInputMethodV2Private::on_commit);
        sc.connect(&handle->events.new_popup_surface, this, &QWInputMethodV2Private::on_new_popup_surface);
        sc.connect(&handle->events.grab_keyboard, this, &QWInputMethodV2Private::on_grab_keyboard);
    }

    void on_commit(void *);
    void on_new_popup_surface(void *);
    void on_grab_keyboard(void *);

    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWInputMethodV2)
};
QHash<void*, QWWrapObject*> QWInputMethodV2Private::map;

void QWInputMethodV2Private::on_commit(void *data)
{
    auto *inputMethod = QWInputMethodV2::from(reinterpret_cast<wlr_input_method_v2*>(data));
    Q_EMIT q_func()->commit(inputMethod);
}

void QWInputMethodV2Private::on_new_popup_surface(void *data)
{
    auto *surface = QWInputPopupSurfaceV2::from(reinterpret_cast<wlr_input_popup_surface_v2*>(data));
    Q_EMIT q_func()->newPopupSurface(surface);
}

void QWInputMethodV2Private::on_grab_keyboard(void *data)
{
    auto *keyboardGrab = QWInputMethodKeyboardGrabV2::from(reinterpret_cast<wlr_input_method_keyboard_grab_v2*>(data));
    Q_EMIT q_func()->grabKeybord(keyboardGrab);
}

QWInputMethodV2::QWInputMethodV2(wlr_input_method_v2 *handle, bool isOwner)
    : QWWrapObject(*new QWInputMethodV2Private(handle, isOwner, this))
{

}

QWInputMethodV2 *QWInputMethodV2::get(wlr_input_method_v2 *handle)
{
    return static_cast<QWInputMethodV2*>(QWInputMethodV2Private::map.value(handle));
}

QWInputMethodV2 *QWInputMethodV2::from(wlr_input_method_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputMethodV2(handle, false);
}

void QWInputMethodV2::sendActivate()
{
    wlr_input_method_v2_send_activate(handle());
}

void QWInputMethodV2::sendContentType(uint32_t hint, uint32_t purpose)
{
    wlr_input_method_v2_send_content_type(handle(), hint, purpose);
}

void QWInputMethodV2::sendDeactivate()
{
    wlr_input_method_v2_send_deactivate(handle());
}

void QWInputMethodV2::sendDone()
{
    wlr_input_method_v2_send_done(handle());
}

void QWInputMethodV2::sendSurroundingText(const char *text, uint32_t cursor, uint32_t anchor)
{
    wlr_input_method_v2_send_surrounding_text(handle(), text, cursor, anchor);
}

void QWInputMethodV2::sendTextChangeCause(uint32_t cause)
{
    wlr_input_method_v2_send_text_change_cause(handle(), cause);
}

void QWInputMethodV2::sendUnavailable()
{
    wlr_input_method_v2_send_unavailable(handle());
}

QW_END_NAMESPACE
