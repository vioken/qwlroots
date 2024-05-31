// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwprimaryselection.h"
#include "private/qwglobal_p.h"

#include <qwseat.h>
#include <QHash>

extern "C" {
#include <wlr/types/wlr_primary_selection.h>
}

QW_BEGIN_NAMESPACE

class QWPrimarySelectionSourcePrivate : public QWWrapObjectPrivate
{
public:
    QWPrimarySelectionSourcePrivate(wlr_primary_selection_source *handle, bool isOwner, QWPrimarySelectionSource *qq)
        : QWWrapObjectPrivate(handle, isOwner, qq, &map, &handle->events.destroy,
                              toDestroyFunction(wlr_primary_selection_source_destroy))
    {

    }
    static QHash<void*, QWWrapObject*> map;
    QW_DECLARE_PUBLIC(QWPrimarySelectionSource)
};
QHash<void*, QWWrapObject*> QWPrimarySelectionSourcePrivate::map;

QWPrimarySelectionSource::QWPrimarySelectionSource(wlr_primary_selection_source *handle, bool isOwner)
    : QWWrapObject(*new QWPrimarySelectionSourcePrivate(handle, isOwner, this))
{

}

QWPrimarySelectionSource *QWPrimarySelectionSource::get(wlr_primary_selection_source *handle)
{
    return static_cast<QWPrimarySelectionSource*>(QWPrimarySelectionSourcePrivate::map.value(handle));
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
