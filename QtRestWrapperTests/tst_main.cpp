#include "tst_qrestwrapper.h"
#include "tst_qrestwrappercookiejartest.h"
#include "tst_qrestwrapperurlinterceptortest.h"

#include <QGuiApplication>
#include <QtTest/qtest.h>

#pragma comment(lib, "QtRestWrapper.lib")

QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_ADD_GPU_BLACKLIST_SUPPORT;
    QTEST_SET_MAIN_SOURCE_PATH;
    QtRestWrapper::QRestWrapperTest tc1;
    QtRestWrapper::QRestWrapperCookieJarTest tc2;
    QtRestWrapper::QRestWrapperUrlInterceptorTest tc3;
    int testResult = QTest::qExec(&tc1, argc, argv);
    testResult = QTest::qExec(&tc2, argc, argv);
    testResult = QTest::qExec(&tc3, argc, argv);
    return testResult;
}
