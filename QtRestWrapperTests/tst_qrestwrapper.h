#ifndef TST_QRESTWRAOOER_H
#define TST_QRESTWRAOOER_H

#include <QEvent>
#include <QMainWindow>
#include <QObject>
#include "../QtRestWrapper/qrestwrapper.h"

namespace QtRestWrapper {

class TempWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TempWindow(QWidget *parent = nullptr) : QMainWindow(parent) {}

signals:
    void closed();

protected:
    virtual bool event(QEvent *event) override {
        if(event->type() == QEvent::Close) {
            closed();
        }
        return QMainWindow::event(event);
    }
};

class QRestWrapperTest : public QObject
{
    Q_OBJECT

public:
    QRestWrapperTest();
    ~QRestWrapperTest();

private slots:
    void initTestCase();
    void setApplicationUrlTest();
    void setStoragePathTest();
    void authenticateCustomTest();
    void cleanupTestCase();
public:
    void webengineTest();
    void authenticatorTest();
    void authenticatorTest2();

    void setAuthenticationTestUrlTest();

    //void authenticateCustomTest();
    //void authenticateCustomWithoutWindowTest();
    //void authenticateImplementedTest();

private:
    QRestWrapper *m_restWrapperInstance;
    QRestWrapperPrivate *m_restWrapperPrivateInstance;
    bool m_ssoServerLoaded{false};
    QWidget *m_viewWidget;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAOOER_H
