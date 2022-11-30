// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwxcursormanager.h"
#include "qwcursor.h"
#include "util/qwsignalconnector.h"

extern "C" {
#include <wlr/types/wlr_xcursor_manager.h>
}

QW_BEGIN_NAMESPACE

class QWXCursorManagerPrivate : public QWObjectPrivate
{
public:
    QWXCursorManagerPrivate(void *handle, QWXCursorManager *qq)
        : QWObjectPrivate(handle, qq)
    {

    }
    ~QWXCursorManagerPrivate() {
        sc.invalidate();
        wlr_xcursor_manager_destroy(q_func()->handle());
    }

    QW_DECLARE_PUBLIC(QWXCursorManager)
    QWSignalConnector sc;
};

QWXCursorManager::QWXCursorManager(wlr_xcursor_manager *handle)
    : QWObject(*new QWXCursorManagerPrivate(handle, this))
{

}

QWXCursorManager *QWXCursorManager::create(const char *name, uint32_t size)
{
    auto handle = wlr_xcursor_manager_create(name, size);
    if (!handle)
        return nullptr;
    return new QWXCursorManager(handle);
}

bool QWXCursorManager::load(float scale)
{
    return wlr_xcursor_manager_load(handle(), scale);
}

wlr_xcursor *QWXCursorManager::getXCursor(const char *name, float scale) const
{
    return wlr_xcursor_manager_get_xcursor(handle(), name, scale);
}

void QWXCursorManager::setCursor(const char *name, QWCursor *cursor)
{
    wlr_xcursor_manager_set_cursor_image(handle(), name, cursor->handle());
}

QW_END_NAMESPACE
