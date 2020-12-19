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
#include "eventmonitorthread.h"

#include <QFile>
#include <QTime>
#include <QTextStream>
#include <QThread>

extern "C" {
//#include <stdio.h>
//#include <linux/types.h>
//#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define DEVS_FILE "/proc/bus/input/devices"
}

EventMonitorThread::EventMonitorThread(QMap<QString, QString> keyword) :
    _keyword(keyword)
{

}

EventMonitorThread::~EventMonitorThread(){

}

void EventMonitorThread::run(){

    QString threadText = QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, 16, QLatin1Char('0'));

    qDebug("thread %s begin!", threadText.toLatin1().data());


//    QMap<QString, QString>::iterator it0 = _keyword.begin();
//    for (; it0 != _keyword.end(); it0++){
//        qDebug("this thread's key is:  '%s'", it0.value().toLatin1().data());
//    }

    int fd;
    struct input_event ie;

    //获取EventX
    QString eventFile = "";
    QFile * readFile = new QFile(DEVS_FILE);
    if (!readFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning("Open Devices File '%s' Failed!", DEVS_FILE);
        readFile->close();
        return;
    } else {
        QTextStream stream(readFile);
        QString alldevicesInfo = stream.readAll();
        QStringList devicesInfo = alldevicesInfo.split("\n\n");
        QList<bool> flags;
        for (QString deviceInfo : devicesInfo){
            flags.clear(); flags.append(true);
            QStringList lines = deviceInfo.split("\n", QString::SkipEmptyParts);
            for (QString line : lines){
                QMap<QString, QString>::iterator it = _keyword.begin();
                for (; it != _keyword.end(); it++){
                    if (line.startsWith(it.key()) ){
                        if (line.contains(it.value())){
                            flags.append(true);
                        } else {
                            flags.append(false);
                        }
                    }
                }

                qDebug("debug0: %s", line.toLatin1().data());
                foreach (bool flag, flags) {
                    qDebug("debug1: %d", flag);
                    if (!flag)
                        goto nexttime;
                }

                if (line.startsWith("H: Handlers=")){
                    QString options = line.split("=").at(1);
                    for (QString option : options.split(" ", QString::SkipEmptyParts)){
                        if (option.startsWith("event")){
                            eventFile = "/dev/input/" + option;
                        }
                    }
                }
            }
nexttime:
            if (!eventFile.isEmpty())
                break;
        }
        readFile->close();
    }

    qDebug("Dev EventX is %s", eventFile.toLatin1().data());
    if (eventFile.isEmpty()){
        return;
    }

    QByteArray ba = eventFile.toLatin1();
    fd = open(ba.data(),O_RDONLY);
    if (fd <0 ) {
        qWarning("Open Event File '%s' Failed!", eventFile.toLatin1().data());
        return;
    }

    QTime pressTime(0, 0, 0, 0);
    QTime releaseTime(0, 0, 0, 0);
    QTime longPressTime(0, 0, 0, 0);

    while (1) {
        if (read(fd, &ie, sizeof(ie))){
            if (ie.type == 1){
                qDebug("key event code: %d", ie.code);
                if (ie.code == 40){
                    if (ie.value == 1)
                        pressTime = QTime::currentTime();
                    if (ie.value == 0)
                        releaseTime = QTime::currentTime();
                    if (ie.value == 2)
                        longPressTime = QTime::currentTime();

                    if (pressTime.isValid() && releaseTime.isValid()){
                        emit eventMeet(ie.code);
                        pressTime = QTime(0, 0, 0, 0);
                        releaseTime = QTime(0, 0, 0, 0);
                    }

                }
            }
        }
    }

    close(fd);
}

void EventMonitorThread::callJobComplete(){
    emit jobComplete();
}
