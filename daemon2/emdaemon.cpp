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

}

EMDaemon::~EMDaemon(){

    for (EventMonitorThread * emt : emts){
        emt->callJobComplete();
    }

    delete iface;
}

void EMDaemon:: touchpadToggle(){
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
//            qDebug() << "current name" << deviceinfo.name << deviceinfo.id;
            if (nitems == 1){
                data[0] = (data[0] == 0) ? 1 : 0;

                XChangeDeviceProperty(display, device, prop, XA_INTEGER, realformat, PropModeReplace, data, nitems);
            }

            XFree(data);
        }

        XCloseDevice (display, device);
    }

    if (deviceinfos != NULL)
        XFreeDeviceList (deviceinfos);
}
