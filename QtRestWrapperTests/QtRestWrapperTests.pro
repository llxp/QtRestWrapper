QT += testlib network webenginewidgets
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
    failqnetworkaccessmanager.cpp \
    tst_QRestWrapper.cpp \
    tst_main.cpp \
    tst_qrestwrappercookiejartest.cpp \
    tst_qrestwrapperurlinterceptortest.cpp

HEADERS += \
    failqnetworkaccessmanager.h \
    tst_qrestwrapper.h \
    tst_qrestwrappercookiejartest.h \
    tst_qrestwrapperurlinterceptortest.h

LIBS += -L../build-QtRestWrapper-Desktop_Qt_5_12_3_MSVC2017_64bit-Debug/debug
