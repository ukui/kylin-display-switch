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
#ifndef CLASSREALIZE_H
#define CLASSREALIZE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QTextStream>

class ClassRealize : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "org.ukui.kds.interface")

public:
    explicit ClassRealize();
    ~ClassRealize();

public:
    bool isVirtualWlan(QString dp);

signals:
    Q_SCRIPTABLE void debug0(QString);
    Q_SCRIPTABLE void signalShowTips(int index);
    Q_SCRIPTABLE void signalNextOption();
    Q_SCRIPTABLE void signalLastOption();
    Q_SCRIPTABLE void signalCloseApp();
    Q_SCRIPTABLE void signalMakeClicked();
    Q_SCRIPTABLE void signalButtonClicked(int x, int y);
    Q_SCRIPTABLE void signalMediaKeyTrans(int code);
    Q_SCRIPTABLE void signalRfkillStatusChanged();

public slots:
    Q_SCRIPTABLE void exitService();
    Q_SCRIPTABLE QString helloWorld();
    Q_SCRIPTABLE void helloWorldWithSignal();

    Q_SCRIPTABLE void emitShowTipsSignal(int index);
    Q_SCRIPTABLE void emitNextOption();
    Q_SCRIPTABLE void emitLastOption();
    Q_SCRIPTABLE void emitCloseApp();
    Q_SCRIPTABLE void emitMakeClicked();
    Q_SCRIPTABLE void emitButtonClicked(int x, int y);
    Q_SCRIPTABLE void emitMediaKeyTrans(int code);
    Q_SCRIPTABLE void emitRfkillStatusChanged();

    Q_SCRIPTABLE QString getCameraBusinfo();
    Q_SCRIPTABLE int getCameraDeviceEnable();
    Q_SCRIPTABLE QString toggleCameraDevice(QString businfo);
    Q_SCRIPTABLE int setCameraKeyboardLight(bool lightup);
    Q_SCRIPTABLE int getCurrentFlightMode();
    Q_SCRIPTABLE QList<int> getStatusBeforeFlightModeEnable();
    Q_SCRIPTABLE QString setSingleFlightMode(int type);
    Q_SCRIPTABLE QString toggleFlightMode(bool enable);
    Q_SCRIPTABLE int setAirplaneModeKeyboardLight(bool lightup);
    Q_SCRIPTABLE int getCurrentWlanMode();
    Q_SCRIPTABLE QString toggleWlanMode(bool enable);
    Q_SCRIPTABLE int getCurrentBluetoothMode();
    Q_SCRIPTABLE QString toggleBluetoothMode(bool enable);

};

#endif // CLASSREALIZE_H
