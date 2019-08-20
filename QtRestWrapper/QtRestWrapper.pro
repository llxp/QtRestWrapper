#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T16:05:00
#
#-------------------------------------------------

QT       += gui widgets webengine webenginewidgets core

#QT       -= gui

TARGET = QtRestWrapper
TEMPLATE = lib

DEFINES += QTRESTWRAPPER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/private/qrestwrapperjavascriptinjector.cpp \
        src/qrestwrapper.cpp \
        src/private/qrestwrapper_p.cpp \
        src/private/qrestwrapperauthenticator.cpp \
        src/qrestwrappercertificateerror.cpp \
        src/private/qrestwrappercookiejar.cpp \
        src/private/qrestwrappernetworkhandler.cpp \
        src/private/qrestwrapperpage.cpp \
        src/private/qrestwrapperurlinterceptor.cpp \
        src/private/qrestwrapperutils.cpp \
        src/private/qrestwrapperview.cpp

HEADERS += \
        include/private/qrestwrapperjavascriptinjector.h \
        include/qrestwrapper.h \
        include/private/qrestwrapper_p.h \
        include/private/qrestwrapperauthenticator.h \
        include/qrestwrappercertificateerror.h \
        include/private/qrestwrappercookiejar.h \
        include/private/qrestwrappernetworkhandler.h \
        include/private/qrestwrapperpage.h \
        include/private/qrestwrapperurlinterceptor.h \
        include/private/qrestwrapperutils.h \
        include/private/qrestwrapperview.h \
        include/qtrestwrapper_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/ucrt/x64"
