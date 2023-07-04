#include <QtTest>

#include "qwglobal.h"

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
