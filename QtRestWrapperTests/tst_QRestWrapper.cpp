#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "../QtRestWrapper/qrestwrapper.h"

#include "tst_qrestwrapper.h"

using namespace QtRestWrapper;

QRestWrapperTest::QRestWrapperTest()
{

}

QRestWrapperTest::~QRestWrapperTest()
{

}

void QRestWrapperTest::initTestCase()
{
    m_restWrapperInstance = new QRestWrapper();
    QVERIFY(m_restWrapperInstance != nullptr);
    if(QMetaObject::invokeMethod(m_restWrapperInstance, "d_ptrFunc", Qt::DirectConnection, Q_RETURN_ARG(QRestWrapperPrivate *, m_restWrapperPrivateInstance))) {
        QVERIFY(m_restWrapperPrivateInstance != nullptr);
    } else {
        QVERIFY(false);
    }
}

void QRestWrapperTest::test_case1()
{
}

void QRestWrapperTest::cleanupTestCase()
{
    m_restWrapperInstance->deleteLater();
    m_restWrapperInstance = nullptr;
    QVERIFY(m_restWrapperInstance == nullptr);
}

//QTEST_MAIN(QRestWrapperTest)

//#include "tst_QRestWrapper.moc"
