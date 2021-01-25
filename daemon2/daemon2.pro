QT += core dbus
QT -= gui

CONFIG += link_pkgconfig
CONFIG += c++11

TARGET = emdaemon
#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

#PKGCONFIG += gio-2.0 \
#             gio-unix-2.0 \

LIBS += -lX11 -lXi


target.source += $$TARGET
target.path = /usr/bin

INSTALLS +=  \
            target \

SOURCES += main.cpp \
    emdaemon.cpp \
    eventmonitorthread.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    emdaemon.h \
    eventmonitorthread.h
