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
    QWXCursorManager() = delete;
    ~QWXCursorManager() = delete;

    void operator delete(QWXCursorManager *p, std::destroying_delete_t);

    wlr_xcursor_manager *handle() const;

    static QWXCursorManager *from(wlr_xcursor_manager *handle);
    static QWXCursorManager *create(const char *name, uint32_t size);

    bool load(float scale);
    wlr_xcursor *getXCursor(const char *name, float scale) const;
#if WLR_VERSION_MINOR <= 16
    void setCursor(const char *name, QWCursor *cursor);
#endif
};

QW_END_NAMESPACE
