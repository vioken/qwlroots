// Copyright (C) 2023 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtTest>

#include "qw_global.h"

class testQWObject : public QObject
{
    Q_OBJECT
public:
    testQWObject();
    ~testQWObject() override = default;

private Q_SLOTS:
    void testcase();

private:
    QWLRoots::QWObject* nullObject;
};

testQWObject::testQWObject()
    : nullObject(nullptr)
{
}

void testQWObject::testcase() {
    QCOMPARE(nullObject->isValid(), false);
}

QTEST_APPLESS_MAIN(testQWObject)

#include "test_qwobject.moc"
