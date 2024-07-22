// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <qwglobal.h>

extern "C" {
#include <wlr/util/box.h>
}

#include <QRect>

QW_BEGIN_NAMESPACE

class qw_box: public QRect
{
public:
    explicit qw_box() = default;
    explicit qw_box(const wlr_box &box): QRect(box.x, box.y, box.width, box.height)
    {
    }

    Q_ALWAYS_INLINE static wlr_box toWlrBox(const QRect& box) {
        return wlr_box {
            .x = box.x(),
            .y = box.y(),
            .width = box.width(),
            .height = box.height()
        };
    }

    Q_ALWAYS_INLINE wlr_box toWlrBox() {
        return toWlrBox(*this);
    }
};

class qw_fbox: public QRectF
{
public:
    explicit qw_fbox() = default;
    explicit qw_fbox(const wlr_fbox &box): QRectF(box.x, box.y, box.width, box.height)
    {
    }

    Q_ALWAYS_INLINE static wlr_fbox toWlrFBox(const QRectF& box) {
        return wlr_fbox {
            .x = box.x(),
            .y = box.y(),
            .width = box.width(),
            .height = box.height()
        };
    }

    Q_ALWAYS_INLINE wlr_fbox toWlrFBox() {
        return toWlrFBox(*this);
    }
};

QW_END_NAMESPACE
