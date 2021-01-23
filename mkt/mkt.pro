#-------------------------------------------------
#
# Project created by QtCreator 2020-11-12T15:38:52
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mktip
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


include (../shared/qtsingleapplication/qtsingleapplication.pri)
include (../utils/utils.pri)

LIBS += -lX11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

target.source += $$TARGET
target.path = /usr/bin

INSTALLS +=  \
            target \


SOURCES += \
        main.cpp \
        widget.cpp

HEADERS += \
        widget.h

FORMS += \
        widget.ui

RESOURCES += \
    res/img.qrc

TRANSLATIONS +=  \
                res/zh_CN.ts \
