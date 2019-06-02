#ifndef TST_QRESTWRAOOER_H
#define TST_QRESTWRAOOER_H

#include <QObject>
#include "../QtRestWrapper/qrestwrapper.h"

namespace QtRestWrapper {


class QRestWrapperTest : public QObject
{
    Q_OBJECT

public:
    QRestWrapperTest();
    ~QRestWrapperTest();

public slots:
    void initTestCase();
    void setStoragePathTest();
    void setAuthenticationTestUrlTest();
    void setApplicationUrlTest();
    void authenticateCustomTest();
    void authenticateCustomWithoutWindowTest();
    void authenticateImplementedTest();
    void cleanupTestCase();

private:
    QRestWrapper *m_restWrapperInstance;
    QRestWrapperPrivate *m_restWrapperPrivateInstance;
    bool m_ssoServerLoaded{false};
    QWidget *m_viewWidget;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAOOER_H
