// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwinputmethodv2.h"
#include "qwdisplay.h"
#include "util/qwsignalconnector.h"

#include <QHash>

extern "C" {
#define delete delete_c
#include <wlr/types/wlr_input_method_v2.h>
#undef delete
}

QW_BEGIN_NAMESPACE

class QWInputMethodManagerV2Private : public QWObjectPrivate
{
public:
    QWInputMethodManagerV2Private(wlr_input_method_manager_v2 *handle, bool isOwner, QWInputMethodManagerV2 *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWInputMethodManagerV2Private::on_destroy);
        sc.connect(&handle->events.input_method, this, &QWInputMethodManagerV2Private::on_input_method);
    }
    ~QWInputMethodManagerV2Private() {
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
    void on_input_method(void *);

    static QHash<void*, QWInputMethodManagerV2*> map;
    QW_DECLARE_PUBLIC(QWInputMethodManagerV2)
    QWSignalConnector sc;
};
QHash<void*, QWInputMethodManagerV2*> QWInputMethodManagerV2Private::map;

void QWInputMethodManagerV2Private::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

void QWInputMethodManagerV2Private::on_input_method(void *data)
{
    auto *input_method = QWInputMethodV2::from(reinterpret_cast<wlr_input_method_v2*>(data));
    Q_EMIT q_func()->inputMethod(input_method);
}

QWInputMethodManagerV2::QWInputMethodManagerV2(wlr_input_method_manager_v2 *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWInputMethodManagerV2Private(handle, isOwner, this))
{

}

QWInputMethodManagerV2 *QWInputMethodManagerV2::get(wlr_input_method_manager_v2 *handle)
{
    return QWInputMethodManagerV2Private::map.value(handle);
}

QWInputMethodManagerV2 *QWInputMethodManagerV2::from(wlr_input_method_manager_v2 *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWInputMethodManagerV2(handle, false);
}

QWInputMethodManagerV2 *QWInputMethodManagerV2::create(QWDisplay *display)
{
    auto *handle = wlr_input_method_manager_v2_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWInputMethodManagerV2(handle, true);
}

QW_END_NAMESPACE
