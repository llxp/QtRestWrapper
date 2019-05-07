#ifndef TST_QRESTWRAOOER_H
#define TST_QRESTWRAOOER_H

#include <QObject>

namespace QtRestWrapper {


class QRestWrapperTest : public QObject
{
    Q_OBJECT

public:
    QRestWrapperTest();
    ~QRestWrapperTest();

private slots:
    void test_case1();
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAOOER_H
