#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T11:59:43
#
#-------------------------------------------------

QT       += core webenginewidgets widgets
QT       -= gui

Release:DESTDIR = release
Debug:DESTDIR = debug

Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

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
        c_structures/map_capi.h \
        c_structures/qnetworkcookie_capi.h \
        c_structures/qrestwrappercertificateerror_c.h \
        c_structures/qurl_c.h \
        c_structures/qvariant_capi.h \
        qtrestwrappercapi.h \
        qtrestwrappercapi_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += ../QtRestWrapper/include
LIBS += -L../../QtRestWrapper/bin -lQtRestWrapper

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17134.0/ucrt/x64"
LIBS += -L"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64"
