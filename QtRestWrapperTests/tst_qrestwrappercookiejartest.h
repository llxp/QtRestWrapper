#ifndef TST_QRESTWRAPPERCOOKIEJARTEST_H
#define TST_QRESTWRAPPERCOOKIEJARTEST_H

#include <QObject>

namespace QtRestWrapper {

class QRestWrapperCookieJarTest : public QObject
{
    Q_OBJECT
public:
    QRestWrapperCookieJarTest(QObject *parent = nullptr);

private slots:
    void test_case1();
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAPPERCOOKIEJARTEST_H
