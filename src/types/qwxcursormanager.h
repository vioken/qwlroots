// Copyright (C) 2022 JiDe Zhang <zccrs@live.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

struct wlr_xcursor_manager;
struct wlr_xcursor;

QW_BEGIN_NAMESPACE

class QWCursor;
class QW_EXPORT QWXCursorManager
{
public:
    void destroy();
    wlr_xcursor_manager *handle() const;

    static QWXCursorManager *from(wlr_xcursor_manager *handle);
    static QWXCursorManager *create(const char *name, uint32_t size);

    bool load(float scale);
    wlr_xcursor *getXCursor(const char *name, float scale) const;
    void setCursor(const char *name, QWCursor *cursor);

private:
    QWXCursorManager() = default;
    ~QWXCursorManager() = default;
};

QW_END_NAMESPACE
