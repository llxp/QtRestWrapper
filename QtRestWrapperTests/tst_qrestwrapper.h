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

private slots:
    void initTestCase();
    void test_case1();
    void cleanupTestCase();

private:
    QRestWrapper *m_restWrapperInstance;
    QRestWrapperPrivate *m_restWrapperPrivateInstance;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAOOER_H
