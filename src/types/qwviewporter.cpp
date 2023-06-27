// Copyright (C) 2023 rewine <luhongxu@deepin.org>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwviewporter.h"
#include "util/qwsignalconnector.h"
#include "qwdisplay.h"

#include <QHash>

extern "C" {
#include <wlr/types/wlr_viewporter.h>
}

QW_BEGIN_NAMESPACE

class QWViewPorterPrivate : public QWObjectPrivate
{
public:
    QWViewPorterPrivate(wlr_viewporter *handle, bool isOwner, QWViewPorter *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWViewPorterPrivate::on_destroy);
    }
    ~QWViewPorterPrivate() {
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

    static QHash<void*, QWViewPorter*> map;
    QW_DECLARE_PUBLIC(QWViewPorter)
    QWSignalConnector sc;
};
QHash<void*, QWViewPorter*> QWViewPorterPrivate::map;

void QWViewPorterPrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWViewPorter::QWViewPorter(wlr_viewporter *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWViewPorterPrivate(handle, isOwner, this))
{

}

QWViewPorter *QWViewPorter::get(wlr_viewporter *handle)
{
    return QWViewPorterPrivate::map.value(handle);
}

QWViewPorter *QWViewPorter::from(wlr_viewporter *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWViewPorter(handle, false);
}

QWViewPorter *QWViewPorter::create(QWDisplay *display)
{
    auto *handle = wlr_viewporter_create(display->handle());
    if (!handle)
        return nullptr;
    return new QWViewPorter(handle, true);
}

QW_END_NAMESPACE
