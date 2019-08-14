#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T11:59:43
#
#-------------------------------------------------

QT       += core webenginewidgets widgets
QT       -= gui

TARGET = QtRestWrapperCAPI
TEMPLATE = lib

DEFINES += QTRESTWRAPPERCAPI_LIBRARY

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
        c_structures/qrestwrappercertificateerror_capi.cpp \
        c_structures/qurl_capi.cpp \
        qtrestwrappercapi.cpp

HEADERS += \
        c_structures/qrestwrappercertificateerror_c.h \
        c_structures/qurl_c.h \
        qtrestwrappercapi.h \
        qtrestwrappercapi_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += ../QtRestWrapper/include
LIBS += -L../build-QtRestWrapper-Desktop_Qt_5_13_0_MSVC2017_64bit-Debug/debug -lQtRestWrapper
