// Copyright (C) 2022 zccrs zccrs@live.com.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_xcursor_manager;
struct wlr_xcursor;

QW_BEGIN_NAMESPACE

class QWCursor;
class QWXCursorManagerPrivate;
class QW_EXPORT QWXCursorManager : public QWObject
{
    QW_DECLARE_PRIVATE(QWXCursorManager)
public:
    explicit QWXCursorManager(wlr_xcursor_manager *handle);

    inline wlr_xcursor_manager *handle() const {
        return QWObject::handle<wlr_xcursor_manager>();
    }

    static QWXCursorManager *create(const char *name, uint32_t size);

    bool load(float scale);
    wlr_xcursor *getXCursor(const char *name, float scale) const;
    void setCursor(const char *name, QWCursor *cursor);
};

QW_END_NAMESPACE
