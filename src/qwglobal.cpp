// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwobject.h"

namespace qw {
QHash<void*, QObject*> &map() {
    static QHash<void*, QObject*> map;
    return map;
}
}
