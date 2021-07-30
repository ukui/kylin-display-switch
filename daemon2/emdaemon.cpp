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
#include "emdaemon.h"

#include <X11/extensions/XInput.h>
#include <X11/Xatom.h>

EMDaemon::EMDaemon()
{

    iface = new QDBusInterface("org.ukui.kds", \
                               "/", \
                               "org.ukui.kds.interface", \
                               QDBusConnection::systemBus());

    QMap<QString, QString> keyWord1;
    QMap<QString, QString> keyWord2;
    QMap<QString, QString> keyWord3;
    keyWord1.insert("N: Name", "2 keyboard");
//    keyWord1.insert("P: Phys", "input0");
    keyWord2.insert("N: Name", "Hotkey");
    keyWord3.insert("N: Name", "SCI_EVT");

    QList<QMap<QString, QString>> filters;
    filters.append(keyWord1);
    filters.append(keyWord2);
    filters.append(keyWord3);


    for (QMap<QString, QString> k : filters){
        QThread * thrd = new QThread;
        EventMonitorThread * emt = new EventMonitorThread(k);

        emts.append(emt);

        connect(emt, &EventMonitorThread::eventMeet, this, [=](int code){

            iface->call("emitMediaKeyTrans", code);

        }, Qt::QueuedConnection);

        connect(emt, &EventMonitorThread::jobComplete, this, [=]{
            thrd->quit(); //退出事件循环
            thrd->wait(); //释放资源
        });

        connect(thrd, &QThread::started, emt, &EventMonitorThread::run);

        connect(thrd, &QThread::finished, emt, &EventMonitorThread::deleteLater);


        emt->moveToThread(thrd);

        thrd->start();
    }


    //rfkill monitor start
    thrd2 = new QThread;
    rmt = new RfkillMonitorthread;

    connect(rmt, &RfkillMonitorthread::wlanStatusChanged, this, [=](int current){
        // WLANON: 12; WLANOFF: 13;
        current ? iface->call("emitShowTipsSignal", 13) : iface->call("emitShowTipsSignal", 12);
    }, Qt::QueuedConnection);

    connect(rmt, &RfkillMonitorthread::jobComplete, this, [=]{
        thrd2->quit();
        thrd2->wait();
    });

    connect(thrd2, &QThread::started, rmt, &RfkillMonitorthread::run);
    connect(thrd2, &QThread::finished, rmt, &RfkillMonitorthread::deleteLater);

    rmt->moveToThread(thrd2);

    thrd2->start();

}

EMDaemon::~EMDaemon(){

    for (EventMonitorThread * emt : emts){
        emt->callJobComplete();
    }

    rmt->callJobComplete();

    delete iface;
}
