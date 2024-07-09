// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwabc.h"

#include <QtTest>

class testQWObject : public QObject
{
    Q_OBJECT
public:
    testQWObject() = default;
    ~testQWObject() override = default;

private Q_SLOTS:
    void initTestCase();
    void testfunc();
    void testsignal();
    void cleanupTestCase();

private:
    wl_display *display;
    qw_abc* qabc;
    const char *notified_name { nullptr };
    bool qabc_destroyed { false };
};

void testQWObject::initTestCase() {
    display = wl_display_create();
    qabc = qw_abc::create(display);
    connect(qabc, &qw_abc::notify_set_name, this, [this](const char *name){
        notified_name = name;
    });
    connect(qabc, &qw_abc::before_destroy, this, [this](){
        qabc_destroyed = true;
    });
}

void testQWObject::testfunc() {
    (*qabc)->a = 1;
    (*qabc)->b = 2;
    (*qabc)->c = 3;
    QCOMPARE(qabc->sum(), 6);
    (*qabc)->a = 100;
    QCOMPARE(qabc->sum(), 105);
}

void testQWObject::testsignal() {
    char name[] = "just_test";
    qabc->set_name(name);
    QCOMPARE(strcmp(notified_name, name), 0);
}

void testQWObject::cleanupTestCase() {
    wl_display_destroy(display);
    QCOMPARE(qabc_destroyed, true);
}

QTEST_APPLESS_MAIN(testQWObject)

#include "test_qwobject.moc"
