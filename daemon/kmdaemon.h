#ifndef KMDAEMON_H
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
#define KMDAEMON_H

#include <QObject>
#include <QThread>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QGSettings/QGSettings>

#include <QDebug>

#include "mappingtable.h"
#include "keymonitorthread.h"

class KMDaemon : public QObject
{
    Q_OBJECT

public:
    explicit KMDaemon();
    ~KMDaemon();


public:
    void begin();

public:
    bool getCurrentCapslockStatus();
    bool getCurrentNumlockStatus();

    void touchpadToggle();
    void touchpadToggle2(bool enable);
    void microphoneToggle();
    void cameraToggle();
    void flightToggle();
    void wlanToggle();
    void screenLock();
    void screenToggle();

    void keyboardLightInit();
    void airplanModeKeyLightInit();
    void disableCameraKeyLightInit();

private:
    QThread * thrd;
    KeyMonitorThread * kmt;

    QDBusInterface * iface;
    QDBusInterface * ifaceScreenSaver;

private:
    bool modifyKeyPressed;

    bool capslockStatus;
    bool numlockStatus;

    bool cameraEnableStatus;

    QGSettings * settings;
    QGSettings * kbGSettings;

    bool stInstalled;
    bool kbInstalled;

public slots:
    void mediaKeyManager(int code);

};

#endif // KMDAEMON_H
