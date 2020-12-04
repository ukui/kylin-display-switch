QT += core dbus
QT -= gui

CONFIG += c++11
CONFIG += link_pkgconfig

TARGET = registerdbus
CONFIG -= app_bundle

TEMPLATE = app
TARGET = launchdbus
DESTDIR = .



target.source += $$TARGET
target.path = /usr/bin
service.files += conf/org.ukui.kds.service
service.path = /usr/share/dbus-1/system-services/
conffile.files += conf/org.ukui.kds.conf
conffile.path = /etc/dbus-1/system.d/


INSTALLS +=  \
            target \
            service \
            conffile


SOURCES += main.cpp \
    classrealize.cpp

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
    classrealize.h
