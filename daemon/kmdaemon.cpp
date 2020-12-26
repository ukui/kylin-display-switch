/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "kmdaemon.h"

#include <X11/keysym.h>
#include <QProcess>

#include <X11/extensions/XInput.h>
#include <X11/Xatom.h>

#define UKCCOSD_SCHEMA "org.ukui.control-center.osd"
#define KYCCOSD_SCHEMA "org.kylin.control-center.osd"
#define KDSOSD_SCHEMA "org.ukui.kds.osd"

#define UK_TOUCHPAD_SCHEMA "org.ukui.peripherals-touchpad"
#define KY_TOUCHPAD_SCHEMA "org.mate.peripherals-touchpad"

#define SHOW_TIP_KEY "show-lock-tip"
#define RUNNING_KEY "running"
#define TP_ENABLE_KEY "touchpad-enabled"
#define MP_ENABLE_KEY "microphone"

KMDaemon::KMDaemon()
{
    thrd = new QThread;
    kmt = new KeyMonitorThread;
    iface = new QDBusInterface("org.ukui.kds", \
                               "/", \
                               "org.ukui.kds.interface", \
                               QDBusConnection::systemBus());

    modifyKeyPressed = false;
    stInstalled = true;

    capslockStatus = getCurrentCapslockStatus();
    numlockStatus = getCurrentNumlockStatus();

    const QByteArray id(UKCCOSD_SCHEMA);
    const QByteArray idd(KYCCOSD_SCHEMA);
    const QByteArray iid(KDSOSD_SCHEMA);

    if (QGSettings::isSchemaInstalled(id)){
        settings = new QGSettings(id);
    } else if (QGSettings::isSchemaInstalled(idd)){
        settings = new QGSettings(idd);
    } else if (QGSettings::isSchemaInstalled(iid)){
        settings = new QGSettings(iid);
    } else {
        stInstalled = false;
    }

    //KeyCode 比 正常键值大 8，原因未知
    connect(kmt, &KeyMonitorThread::keyPress, this, [=](KeySym mks, KeyCode mkc){
//        qDebug() << "key press:" << mkc - 8;

//        if (!iface->isValid()){
//            qCritical() << "Create Client Interface Failed When execute chage: " << QDBusConnection::systemBus().lastError();
//            return;
//        }

        if (mks == XK_Super_L || mks== XK_Super_R){
            modifyKeyPressed = true;
        } else if (mks == XK_p){
            if (modifyKeyPressed){
//                qDebug() << "win + p" << "pressed";

                QProcess process;
                QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                env.insert("DISPLAY", ":0");
                process.setProcessEnvironment(env);
                process.startDetached("/usr/bin/kydisplayswitch");

            } else {
                iface->call("emitCloseApp");
            }

        } else if (mks == XK_KP_Enter || mks == XK_Return){
//            qDebug() << "enter is pressed!";
            iface->call("emitMakeClicked");

        } else if (mks == XK_Up || mks == XK_KP_Up) {
            iface->call("emitLastOption");

        } else if (mks == XK_Down || mks == XK_KP_Down) {
            iface->call("emitNextOption");

        } else if (mks == XK_Caps_Lock){
            if (stInstalled && !settings->get(SHOW_TIP_KEY).toBool()){
                return;
            }

            if (capslockStatus){
                iface->call("emitShowTipsSignal", 1);
            } else {
                iface->call("emitShowTipsSignal", 0);
            }

            capslockStatus = !capslockStatus;
        } else if (mks == XK_Num_Lock){
            if (stInstalled && !settings->get(SHOW_TIP_KEY).toBool()){
                return;
            }

            if (numlockStatus){
                iface->call("emitShowTipsSignal", 3);
            } else {
                iface->call("emitShowTipsSignal", 2);
            }

            numlockStatus = !numlockStatus;
        }else {
            iface->call("emitCloseApp");
        }

    }, Qt::QueuedConnection);

    connect(kmt, &KeyMonitorThread::keyRelease, this, [=](KeySym mks, KeyCode mkc){
//        qDebug() << "key release:" << mkc - 8;
        if (mks == XK_Super_L || mks== XK_Super_R){
            modifyKeyPressed = false;
        }
    }, Qt::QueuedConnection);


    connect(kmt, &KeyMonitorThread::buttonPress, this, [=] (int x, int y) {
//        qDebug() << "button press" << x << y;
        iface->call("emitButtonClicked", x, y);
    },
    Qt::QueuedConnection);

    connect(kmt, &KeyMonitorThread::jobComplete, this, [=]{
        thrd->quit(); //退出事件循环
        thrd->wait(); //释放资源
    });

    connect(thrd, &QThread::started, kmt, &KeyMonitorThread::run);

    connect(thrd, &QThread::finished, kmt, &KeyMonitorThread::deleteLater);

    //绑定特殊
    QDBusConnection::systemBus().connect(QString(), QString(), "org.ukui.kds.interface", "signalMediaKeyTrans", this, SLOT(mediaKeyManager(int)));

}

KMDaemon::~KMDaemon()
{
    kmt->callJobComplete();
    delete iface;

    if (stInstalled)
        delete settings;
}

void KMDaemon::begin(){
    qDebug("thread begin!");

    kmt->moveToThread(thrd);

    thrd->start();
}

bool KMDaemon::getCurrentCapslockStatus(){
    QString cmd = "xset q";
    QProcess process;
    process.start(cmd);
    process.waitForFinished();

    QByteArray ba = process.readAllStandardOutput();
    QString output = QString(ba.data()).simplified();

    QString flag = "Caps Lock:";
//    qDebug() << "outputis : " << output;
    QString status = output.mid(output.indexOf(flag) + 11, 3).simplified();
//    qDebug() << "status is : " << status;

    return status == "on" ? true : false;
}

bool KMDaemon::getCurrentNumlockStatus(){
    QString cmd = "xset q";
    QProcess process;
    process.start(cmd);
    process.waitForFinished();

    QByteArray ba = process.readAllStandardOutput();
    QString output = QString(ba.data()).simplified();

    QString flag = "Num Lock:";
//    qDebug() << "outputis : " << output;
    QString status = output.mid(output.indexOf(flag) + 10, 3).simplified();
//    qDebug() << "status is : " << status;

    return status == "on" ? true : false;
}

void KMDaemon::mediaKeyManager(int code){

    qDebug() << "kmdaemon receive: " << code;
    switch (code) {
    case 530:
        touchpadToggle();
        break;
    case 531:
        touchpadToggle2(true);
        break;
    case 532:
        touchpadToggle2(false);
    case 248:
        microphoneToggle();
        break;
    default:
        break;
    }
}

void KMDaemon::touchpadToggle(){
    XDeviceInfo *deviceinfos;
    int n_devices;
    int realformat;
    unsigned long nitems, bytes_after;
    unsigned char *data;

    Display * display = XOpenDisplay(0);

    deviceinfos = XListInputDevices (display, &n_devices);

    if (deviceinfos == NULL)
        return;

    for (int i = 0; i < n_devices; i++){
        XDevice * device;
        Atom realtype, prop;
        XDeviceInfo deviceinfo = deviceinfos[i];

        if (deviceinfo.type != XInternAtom (display, XI_MOUSE, False)){
            continue;
        }

        prop = XInternAtom (display, "Device Enabled", False);

        if (!prop)
            continue;

        device = XOpenDevice (display, deviceinfo.id);

        if (!device)
            continue;

        if (XGetDeviceProperty (display, device, prop, 0, 1, False,
                                XA_INTEGER, &realtype, &realformat, &nitems,
                                &bytes_after, &data) == Success) {

            if (QString(deviceinfo.name).contains("USB Optical Mouse"))
                continue;
            qDebug() << "current name" << deviceinfo.name << deviceinfo.id;
            if (nitems == 1){
//                data[0] = (data[0] == 0) ? 1 : 0;

                if (data[0] == 1){
                    QString cmd = QString("xinput disable %1").arg(QString::number((int)deviceinfo.id));
                    qDebug() << "run" << cmd;
                    system(cmd.toLatin1().data());
                    iface->call("emitShowTipsSignal", 5);
                } else {
                    QString cmd = QString("xinput enable %1").arg(QString::number((int)deviceinfo.id));
                    qDebug() << "run" << cmd;
                    system(cmd.toLatin1().data());
                    iface->call("emitShowTipsSignal", 4);
                }

//                XChangeDeviceProperty(display, device, prop, XA_INTEGER, realformat, PropModeReplace, data, nitems);
            }

            XFree(data);
        }

        XCloseDevice (display, device);
    }

    if (deviceinfos != NULL)
        XFreeDeviceList (deviceinfos);
}

void KMDaemon::touchpadToggle2(bool enable){
    const QByteArray id(UK_TOUCHPAD_SCHEMA);
    const QByteArray idd(KY_TOUCHPAD_SCHEMA);
    if (QGSettings::isSchemaInstalled(id)){
        QGSettings * st = new QGSettings(id);
        st->set(TP_ENABLE_KEY, enable);
        if (enable){
            iface->call("emitShowTipsSignal", 4);
        } else {
            iface->call("emitShowTipsSignal", 5);
        }
        delete st;
    } else if (QGSettings::isSchemaInstalled(idd)){
        QGSettings * st = new QGSettings(idd);
        st->set(TP_ENABLE_KEY, enable);
        if (enable){
            iface->call("emitShowTipsSignal", 4);
        } else {
            iface->call("emitShowTipsSignal", 5);
        }
        delete st;
    }
}

void KMDaemon::microphoneToggle(){
    const QByteArray id(KDSOSD_SCHEMA);
    if (QGSettings::isSchemaInstalled(id)){
        QGSettings * st = new QGSettings(id);
        bool current = st->get(MP_ENABLE_KEY).toBool();

        st->set(MP_ENABLE_KEY, !current);

        if (current){
            iface->call("emitShowTipsSignal", 4);
        } else {
            iface->call("emitShowTipsSignal", 4);
        }
    }
}
