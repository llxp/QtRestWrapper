#include "tst_qrestwrapperurlinterceptortest.h"
#include "../QtRestWrapper/qrestwrapperurlinterceptor.h"

using namespace QtRestWrapper;

#include <QtTest>

class QWebEngineUrlRequestInfoPrivate : public QObject
{
    Q_OBJECT
public:
    QWebEngineUrlRequestInfoPrivate(QObject *parent = nullptr) : QObject(parent)
    {

    }
};

QRestWrapperUrlInterceptorTest::QRestWrapperUrlInterceptorTest(QObject *parent) : QObject(parent)
{

}

void QRestWrapperUrlInterceptorTest::initTestCase()
{
    m_requestInterceptor = new QRestWrapperUrlInterceptor();
    QVERIFY(m_requestInterceptor != nullptr);
    m_restWrapperInstance = new QRestWrapper();
    QVERIFY(m_restWrapperInstance != nullptr);
    if(QMetaObject::invokeMethod(m_restWrapperInstance, "d_ptrFunc", Qt::DirectConnection, Q_RETURN_ARG(QRestWrapperPrivate *, m_restWrapperPrivateInstance))) {
        QVERIFY(true);
    } else {
        QVERIFY(false);
    }
    QVERIFY(m_restWrapperPrivateInstance != nullptr);
}

void QRestWrapperUrlInterceptorTest::setRestWrapper_case()
{
    m_requestInterceptor->setRestWrapper(m_restWrapperPrivateInstance);
    QVERIFY(m_requestInterceptor->m_restWrapper == m_restWrapperPrivateInstance);
}

void QRestWrapperUrlInterceptorTest::checkForAllowedList_case()
{
    QVector<QString> allowedList;
    allowedList.append("https:\\/\\/www\\.google\\.de\\/api\\/.*");
    allowedList.append("https:\\/\\/www\\.google\\.de\\/img\\/.*");
    allowedList.append("https:\\/\\/www\\..*\\.de\\/api\\/.*");
    QVERIFY(m_requestInterceptor->checkForAllowedList(allowedList, QUrl("https://www.google.de/api/test?var=1")) == true);
    QVERIFY(m_requestInterceptor->checkForAllowedList(allowedList, QUrl("https://www.google.de/img/test2?var=1")) == true);
    QVERIFY(m_requestInterceptor->checkForAllowedList(allowedList, QUrl("https://www.google.com/api/test?var=1")) == false);
    QVERIFY(m_requestInterceptor->checkForAllowedList(allowedList, QUrl("https://www.google.com/img/test2?var=1")) == false);
}

void QRestWrapperUrlInterceptorTest::checkForForbiddenList_case()
{
    QVector<QString> forbiddenList;
    forbiddenList.append("https:\\/\\/www\\.google\\.de\\/api\\/.*");
    forbiddenList.append("https:\\/\\/www\\.google\\.de\\/img\\/.*");
    QVERIFY(m_requestInterceptor->checkForForbiddenList(forbiddenList, QUrl("https://www.google.de/api/test?var=1")) == false);
    QVERIFY(m_requestInterceptor->checkForForbiddenList(forbiddenList, QUrl("https://www.google.de/img/test2?var=1")) == false);
    QVERIFY(m_requestInterceptor->checkForForbiddenList(forbiddenList, QUrl("https://www.google.com/api/test?var=1")) == true);
    QVERIFY(m_requestInterceptor->checkForForbiddenList(forbiddenList, QUrl("https://www.google.com/img/test2?var=1")) == true);
}

void QRestWrapperUrlInterceptorTest::cleanupTestCase()
{
    m_requestInterceptor->deleteLater();
    m_requestInterceptor = nullptr;
    QVERIFY(m_requestInterceptor == nullptr);
}
