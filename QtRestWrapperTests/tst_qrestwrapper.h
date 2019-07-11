#ifndef TST_QRESTWRAOOER_H
#define TST_QRESTWRAOOER_H

#include <QEvent>
#include <QMainWindow>
#include <QObject>
#include <QProcess>
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
    void webengineTest();
    void authenticatorTest();
    void authenticatorTest2();
    void setApplicationUrlTest();
    void setStoragePathTest();
    void authenticateCustomTest();
    void authenticateRunJavaScriptTest();
    void cleanupTestCase();
public:
    void authenticateCustomWithoutWindowTest();


    void setAuthenticationTestUrlTest();

    //void authenticateCustomTest();

    //void authenticateImplementedTest();

private:
    QRestWrapper *m_restWrapperInstance;
    QRestWrapperPrivate *m_restWrapperPrivateInstance;
    bool m_ssoServerLoaded{false};
    QWidget *m_viewWidget;
    QProcess *m_appProcess;
    QProcess *m_ssoServerProcess;
    QThread *m_appThread;
    QThread *m_ssoServerThread;
};

}  // namespace QtRestWrapper

#endif // TST_QRESTWRAOOER_H
