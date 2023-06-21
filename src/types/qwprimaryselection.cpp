// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwprimaryselection.h"
#include "util/qwsignalconnector.h"

#include <qwseat.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_primary_selection.h>
}

QW_BEGIN_NAMESPACE

class QWPrimarySelectionSourcePrivate : public QWObjectPrivate
{
public:
    QWPrimarySelectionSourcePrivate(wlr_primary_selection_source *handle, bool isOwner, QWPrimarySelectionSource *qq)
        : QWObjectPrivate(handle, isOwner, qq)
    {
        Q_ASSERT(!map.contains(handle));
        map.insert(handle, qq);
        sc.connect(&handle->events.destroy, this, &QWPrimarySelectionSourcePrivate::on_destroy);
    }
    ~QWPrimarySelectionSourcePrivate() {
        if (!m_handle)
            return;
        destroy();
        if (isHandleOwner)
            wlr_primary_selection_source_destroy(q_func()->handle());
    }

    inline void destroy() {
        Q_ASSERT(m_handle);
        Q_ASSERT(map.contains(m_handle));
        Q_EMIT q_func()->beforeDestroy(q_func());
        map.remove(m_handle);
        sc.invalidate();
    }

    void on_destroy(void *);

    static QHash<void*, QWPrimarySelectionSource*> map;
    QW_DECLARE_PUBLIC(QWPrimarySelectionSource)
    QWSignalConnector sc;
};
QHash<void*, QWPrimarySelectionSource*> QWPrimarySelectionSourcePrivate::map;

void QWPrimarySelectionSourcePrivate::on_destroy(void *)
{
    destroy();
    m_handle = nullptr;
    delete q_func();
}

QWPrimarySelectionSource::QWPrimarySelectionSource(wlr_primary_selection_source *handle, bool isOwner)
    : QObject(nullptr)
    , QWObject(*new QWPrimarySelectionSourcePrivate(handle, isOwner, this))
{

}

QWPrimarySelectionSource *QWPrimarySelectionSource::get(wlr_primary_selection_source *handle)
{
    return QWPrimarySelectionSourcePrivate::map.value(handle);
}

QWPrimarySelectionSource *QWPrimarySelectionSource::from(wlr_primary_selection_source *handle)
{
    if (auto o = get(handle))
        return o;
    return new QWPrimarySelectionSource(handle, false);
}

void QWPrimarySelectionSource::init(const wlr_primary_selection_source_impl *impl)
{
    return wlr_primary_selection_source_init(handle(), impl);
}

void QWPrimarySelectionSource::send(const char *mime_type, int fd)
{
    return wlr_primary_selection_source_send(handle(), mime_type, fd);
}

void QWPrimarySelectionSource::requestSetPrimarySelection(QWSeat *seat, wlr_seat_client *client, uint32_t serial)
{
    return wlr_seat_request_set_primary_selection(seat->handle(), client, handle(), serial);
}

void QWPrimarySelectionSource::setPrimarySelection(QWSeat *seat, uint32_t serial)
{
    return wlr_seat_set_primary_selection(seat->handle(), handle(), serial);
}

QW_END_NAMESPACE
