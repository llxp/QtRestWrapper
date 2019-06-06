#ifndef TST_QRESTWRAPPERURLINTERCEPTORTEST_H
#define TST_QRESTWRAPPERURLINTERCEPTORTEST_H

#include <QObject>

#include "../QtRestWrapper/qrestwrappercertificateerror.h"
#include "../QtRestWrapper/private/qrestwrapperauthenticator.h"
#include "../QtRestWrapper/private/qrestwrapperurlinterceptor.h"


namespace QtRestWrapper {

class QRestWrapperUrlInterceptorTest : public QObject
{
    Q_OBJECT
public:
    explicit QRestWrapperUrlInterceptorTest(QObject *parent = nullptr);

signals:

private slots:
    void initTestCase();
    void setRestWrapper_case();
    void checkForAllowedList_case();
    void checkForForbiddenList_case();
    void cleanupTestCase();

private:
    QtRestWrapper::QRestWrapperAuthenticator *m_restWrapperAuthenticatorInstance;
    QtRestWrapper::QRestWrapperUrlInterceptor *m_requestInterceptor;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAPPERURLINTERCEPTORTEST_H
