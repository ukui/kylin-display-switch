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

#define UKCCOSD_SCHEMA "org.ukui.control-center.osd"
#define KYCCOSD_SCHEMA "org.kylin.control-center.osd"
#define KDSOSD_SCHEMA "org.ukui.kds.osd"

#define SHOW_TIP_KEY "show-lock-tip"
#define RUNNING_KEY "running"

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
        qDebug() << "key press:" << mkc - 8;

//        if (!iface->isValid()){
//            qCritical() << "Create Client Interface Failed When execute chage: " << QDBusConnection::systemBus().lastError();
//            return;
//        }

        if (mks == XK_Super_L){
            modifyKeyPressed = true;
        } else if (mks == XK_p){
            if (modifyKeyPressed){
                qDebug() << "win + p" << "pressed";

                QProcess process;
                QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
                env.insert("DISPLAY", ":0");
                process.setProcessEnvironment(env);
                process.startDetached("/usr/bin/kds");

            } else {
                iface->call("emitCloseApp");
            }

        } else if (mks == XK_KP_Enter || mks == XK_Return){
            qDebug() << "enter is pressed!";
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
        qDebug() << "key release:" << mkc - 8;
        if (mks == XK_Super_L){
            modifyKeyPressed = false;
        }
    }, Qt::QueuedConnection);


    connect(kmt, &KeyMonitorThread::buttonPress, this, [=] (int x, int y) {
        qDebug() << "button press" << x << y;
        iface->call("emitButtonClicked", x, y);
    },
    Qt::QueuedConnection);

    connect(kmt, &KeyMonitorThread::jobComplete, this, [=]{
        thrd->quit(); //退出事件循环
        thrd->wait(); //释放资源
    });

    connect(thrd, &QThread::started, kmt, &KeyMonitorThread::run);

    connect(thrd, &QThread::finished, kmt, &KeyMonitorThread::deleteLater);

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
    qDebug() << "outputis : " << output;
    QString status = output.mid(output.indexOf(flag) + 11, 3).simplified();
    qDebug() << "status is : " << status;

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
    qDebug() << "outputis : " << output;
    QString status = output.mid(output.indexOf(flag) + 10, 3).simplified();
    qDebug() << "status is : " << status;

    return status == "on" ? true : false;
}
