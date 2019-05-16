#ifndef TST_QRESTWRAPPERURLINTERCEPTORTEST_H
#define TST_QRESTWRAPPERURLINTERCEPTORTEST_H

#include <QObject>

#include "../QtRestWrapper/qrestwrapper.h"

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
    QtRestWrapper::QRestWrapper *m_restWrapperInstance;
    QRestWrapperPrivate *m_restWrapperPrivateInstance;
    QtRestWrapper::QRestWrapperUrlInterceptor *m_requestInterceptor;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAPPERURLINTERCEPTORTEST_H
