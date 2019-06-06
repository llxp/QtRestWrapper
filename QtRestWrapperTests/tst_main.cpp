#include "tst_qrestwrapper.h"
#include "tst_qrestwrappercookiejartest.h"
#include "tst_qrestwrapperurlinterceptortest.h"

#include <QGuiApplication>
#include <QMainWindow>
#include <QWebEngineProfile>
#include <QtTest/qtest.h>
#include <QLayout>
#include <QSignalSpy>

#include "../QtRestWrapper/private/qrestwrapperview.h"

#pragma comment(lib, "QtRestWrapper.lib")

//QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setAttribute(Qt::AA_Use96Dpi, true);
    //app.setAttribute(Qt::AA_UseOpenGLES);
    //QTEST_ADD_GPU_BLACKLIST_SUPPORT;
    //QTEST_SET_MAIN_SOURCE_PATH;
    QtRestWrapper::QRestWrapperTest tc1;
    //QMetaObject::invokeMethod(&tc1, "webengineTest", Qt::QueuedConnection);
    //QMetaObject::invokeMethod(&tc1, "authenticatorTest", Qt::QueuedConnection);
    //QtRestWrapper::QRestWrapperCookieJarTest tc2;
    //QtRestWrapper::QRestWrapperUrlInterceptorTest tc3;
    int testResult = QTest::qExec(&tc1, argc, argv);
    //testResult = QTest::qExec(&tc2, argc, argv);
    //testResult = QTest::qExec(&tc3, argc, argv);
    return testResult;
    //tc1.initTestCase();
    //tc1.setApplicationUrlTest();
    //tc1.setStoragePathTest();
    //tc1.setAuthenticationTestUrlTest();
    //tc1.authenticateCustomTest();
    //tc1.authenticateCustomWithoutWindowTest();
    //qDebug() << "authenticateCustomWithoutWindowTest done.";
    //tc1.cleanupTestCase();
    //app.exit(0);
    //qApp->exit(0);

    /*QtRestWrapper::TempWindow *window = new QtRestWrapper::TempWindow();
    QWebEngineView *view = new QWebEngineView(window);
    QObject *dummy = new QObject(view);
    //QWebEnginePage *page = new QWebEnginePage(new QWebEngineProfile(window), dummy);
    view->setPage(new QWebEnginePage(new QWebEngineProfile(view), dummy));
    window->setCentralWidget(view);
    //QObject *dummy = new QObject(view);
    //QWebEnginePage *page = new QWebEnginePage(dummy);
    //page->connect(page, &QRestWrapperPage::certificateErrorSignal, [](const QRestWrapperCertificateError &error) {
    //    return true;
    //});
    //view->setPage(page);
    //view->setEnabled(false);
    view->setUrl(QUrl("https://localhost:3000/protectedContent"));
    //view->setEnabled(true);
    //view->show();
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    QThread *thread = new QThread();
    thread->connect(thread, &QThread::started, window, [window]() {
        QSignalSpy spy(window, &QtRestWrapper::TempWindow::closed);
        while(spy.count() == 0) {
            QTest::qWait(200);
        }
        qDebug() << "exited...";
    }, Qt::QueuedConnection);
    thread->start();*/
    return app.exec();
}
