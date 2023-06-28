// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>
#include <QObject>

struct wlr_primary_selection_source_impl;
struct wlr_primary_selection_source;
struct wlr_seat_client;

QW_BEGIN_NAMESPACE

class QWSeat;
class QWPrimarySelectionSourcePrivate;
class QW_EXPORT QWPrimarySelectionSource : public QObject, public QWObject
{
    Q_OBJECT
    QW_DECLARE_PRIVATE(QWPrimarySelectionSource)
public:
    ~QWPrimarySelectionSource() = default;

    inline wlr_primary_selection_source *handle() const {
        return QWObject::handle<wlr_primary_selection_source>();
    }

    static QWPrimarySelectionSource *get(wlr_primary_selection_source *handle);
    static QWPrimarySelectionSource *from(wlr_primary_selection_source *handle);

    void init(const wlr_primary_selection_source_impl *impl);
    void send(const char *mime_type, int fd);
    void requestSetPrimarySelection(QWSeat *seat, wlr_seat_client *client, uint32_t serial);
    void setPrimarySelection(QWSeat *seat, uint32_t serial);

Q_SIGNALS:
    void beforeDestroy(QWPrimarySelectionSource *self);

private:
    QWPrimarySelectionSource(wlr_primary_selection_source *handle, bool isOwner);
};

QW_END_NAMESPACE

