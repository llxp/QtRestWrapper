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
    m_restWrapperAuthenticatorInstance = new QRestWrapperAuthenticator();
    QVERIFY(m_restWrapperAuthenticatorInstance != nullptr);
    m_requestInterceptor = new QRestWrapperUrlInterceptor(m_restWrapperAuthenticatorInstance);
    QVERIFY(m_requestInterceptor != nullptr);
}

void QRestWrapperUrlInterceptorTest::setRestWrapper_case()
{
    QVERIFY(m_requestInterceptor->m_restWrapperAuthenticator == m_restWrapperAuthenticatorInstance);
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
