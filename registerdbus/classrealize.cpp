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

#include <QCoreApplication>
#include <QProcess>

#include <QDebug>

#include "classrealize.h"

extern "C" {

#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>

#define RFKILL_EVENT_SIZE_V1	8

struct rfkill_event {
    __u32 idx;
    __u8  type;
    __u8  op;
    __u8  soft, hard;
};

enum rfkill_operation {
    RFKILL_OP_ADD = 0,
    RFKILL_OP_DEL,
    RFKILL_OP_CHANGE,
    RFKILL_OP_CHANGE_ALL,
};

enum rfkill_type {
    RFKILL_TYPE_ALL = 0,
    RFKILL_TYPE_WLAN,
    RFKILL_TYPE_BLUETOOTH,
    RFKILL_TYPE_UWB,
    RFKILL_TYPE_WIMAX,
    RFKILL_TYPE_WWAN,
    RFKILL_TYPE_GPS,
    RFKILL_TYPE_FM,
    RFKILL_TYPE_NFC,
    NUM_RFKILL_TYPES,
};

}

ClassRealize::ClassRealize()
{

}

ClassRealize::~ClassRealize(){

}

void ClassRealize::exitService(){
    qApp->exit(0);
}

QString ClassRealize::helloWorld(){
    return "hello world!";
}

void ClassRealize::helloWorldWithSignal(){
    emit debug0("hello world");
}

void ClassRealize::emitShowTipsSignal(int index){
    emit signalShowTips(index);
}

void ClassRealize::emitNextOption(){
    emit signalNextOption();
}

void ClassRealize::emitLastOption(){
    emit signalLastOption();
}

void ClassRealize::emitCloseApp(){
    emit signalCloseApp();
}

void ClassRealize::emitMakeClicked(){
    emit signalMakeClicked();
}

void ClassRealize::emitButtonClicked(int x, int y){
    emit signalButtonClicked(x, y);
}

void ClassRealize::emitMediaKeyTrans(int code){
    emit signalMediaKeyTrans(code);
}

void ClassRealize::emitRfkillStatusChanged(){
    emit signalRfkillStatusChanged();
}

QString ClassRealize::getCameraBusinfo(){

    QString path = QString("/sys/bus/usb/devices/");
    QDir dir(path);
    if (!dir.exists()){
        return QString();
    }
    dir.setFilter(QDir::Dirs);
    dir.setSorting(QDir::Name);
    QFileInfoList fileinfoList = dir.entryInfoList();

    for(QFileInfo fileinfo : fileinfoList){
        if (fileinfo.fileName() == "." || fileinfo.fileName() == ".."){
            continue;
        }

        if (fileinfo.fileName().contains(":")){
            continue;
        }

        if (fileinfo.fileName().startsWith("usb")){
            continue;
        }
//        qDebug() << "" << fileinfo.fileName() << fileinfo.absoluteFilePath();
        QDir subdir(fileinfo.absoluteFilePath());
        subdir.setFilter(QDir::Files);
        QFileInfoList fileinfoList2 = subdir.entryInfoList();
        for (QFileInfo fileinfo2 : fileinfoList2){
            if (fileinfo2.fileName() == "product"){
                QFile pfile(fileinfo2.absoluteFilePath());
                if (!pfile.open(QIODevice::ReadOnly | QIODevice::Text))
                    return QString();
                QTextStream pstream(&pfile);
                QString output = pstream.readAll();
//                qDebug() << "output: " << output;
                if (output.contains("camera", Qt::CaseInsensitive)){
                    return fileinfo.fileName();
                }

            }
        }

    }

    return QString();
}

int ClassRealize::getCameraDeviceEnable(){

    QString businfo = getCameraBusinfo();
    if (businfo.isEmpty()){
        char * cmd = "lsusb -t | grep 'Driver=uvcvideo'";
        char output[1024] = "\0";

        FILE * stream;
        if ((stream = popen(cmd, "r")) == NULL){
            return -1;
        }

	int ret;
        if (fread(output, sizeof(char), 1024, stream) <= 0){
	    ret = 0;
        } else {
	    ret = 1;
        }
	fclose(stream);
	return ret;
    }

    int isExists = 0;

    QString path = QString("/sys/bus/usb/drivers/usb/");
    QDir dir(path);
    if (!dir.exists()){
        return -1;
    }
    dir.setFilter(QDir::Dirs);
    dir.setSorting(QDir::Name);
    QFileInfoList fileinfoList = dir.entryInfoList();

    for(QFileInfo fileinfo : fileinfoList){
        if (fileinfo.fileName() == "." || fileinfo.fileName() == ".."){
            continue;
        }

        if (fileinfo.fileName().contains(":")){
            continue;
        }

        if (fileinfo.fileName() == businfo){
            isExists = 1;
        }
    }

    return isExists;
}

QString ClassRealize::toggleCameraDevice(QString businfo){

    QString path = QString("/sys/bus/usb/drivers/usb/");

    int status = getCameraDeviceEnable();

    if (status == -1){
        return QString("Camera Device Not Exists!");
    }

    if (status){
        QString cmd = QString("echo '%1' > %2/unbind").arg(businfo).arg(path);
        system(cmd.toLatin1().data());
        return QString("unbinded");
    } else {
        QString cmd = QString("echo '%1' > %2/bind").arg(businfo).arg(path);
        system(cmd.toLatin1().data());
        return QString("binded");
    }
}

int ClassRealize::getCurrentWlanMode(){
    struct rfkill_event event;
    ssize_t len;
    int fd;
    int bls = 0, unbls = 0;

    QList<int> status;

    fd = open("/dev/rfkill", O_RDONLY);
    if (fd < 0) {
        qCritical("Can't open RFKILL control device");
        return -1;
    }

    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        qCritical("Can't set RFKILL control device to non-blocking");
        close(fd);
        return -1;
    }

    while (1) {
        len = read(fd, &event, sizeof(event));
        if (len < 0) {
            if (errno == EAGAIN)
                break;
            qWarning("Reading of RFKILL events failed");
            break;
        }

        if (len != RFKILL_EVENT_SIZE_V1) {
            qWarning("Wrong size of RFKILL event\n");
            continue;
        }

//        printf("%u - %u: %u\n", event.idx, event.type, event.soft);
        if (event.type != RFKILL_TYPE_WLAN)
            continue;

        status.append(event.soft ? 1 : 0);
    }

    close(fd);

    if (status.length() == 0){
        return -1;
    }

    for (int s : status){
        s ? bls++ : unbls++;
    }

    if (bls == status.length()){ //block
        return 0;
    } else if (unbls == status.length()){ //unblock
        return 1;
    } else { //not block & not unblock
        return 0;
    }
}

QString ClassRealize::toggleWlanMode(bool enable){

    struct rfkill_event event;
    int fd;
    ssize_t len;

    __u8 block;

    fd = open("/dev/rfkill", O_RDWR);
    if (fd < 0) {
        return QString("Can't open RFKILL control device");
    }

    block = enable ? 0 : 1;

    memset(&event, 0, sizeof(event));

    event.op= RFKILL_OP_CHANGE_ALL;

    event.type = RFKILL_TYPE_WLAN;

    event.soft = block;

    len = write(fd, &event, sizeof(event));

    if (len < 0){
        return QString("Failed to change RFKILL state");
    }

    close(fd);
    return block ? QString("blocked") : QString("unblocked");
}

int ClassRealize::getCurrentBluetoothMode(){
    struct rfkill_event event;
    ssize_t len;
    int fd;
    int bls = 0, unbls = 0;

    QList<int> status;

    fd = open("/dev/rfkill", O_RDONLY);
    if (fd < 0) {
        qCritical("Can't open RFKILL control device");
        return -1;
    }

    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        qCritical("Can't set RFKILL control device to non-blocking");
        close(fd);
        return -1;
    }

    while (1) {
        len = read(fd, &event, sizeof(event));
        if (len < 0) {
            if (errno == EAGAIN)
                break;
            qWarning("Reading of RFKILL events failed");
            break;
        }

        if (len != RFKILL_EVENT_SIZE_V1) {
            qWarning("Wrong size of RFKILL event\n");
            continue;
        }

//        printf("%u - %u: %u\n", event.idx, event.type, event.soft);
        if (event.type != RFKILL_TYPE_BLUETOOTH)
            continue;

        status.append(event.soft ? 1 : 0);
    }

    close(fd);

    if (status.length() == 0){
        return -1;
    }

    for (int s : status){
        s ? bls++ : unbls++;
    }

    if (bls == status.length()){ //block
        return 0;
    } else if (unbls == status.length()){ //unblock
        return 1;
    } else { //not block & not unblock
        return 0;
    }
}

QString ClassRealize::toggleBluetoothMode(bool enable){
    struct rfkill_event event;
    int fd;
    ssize_t len;

    __u8 block;

    fd = open("/dev/rfkill", O_RDWR);
    if (fd < 0) {
        return QString("Can't open RFKILL control device");
    }

    block = enable ? 0 : 1;

    memset(&event, 0, sizeof(event));

    event.op= RFKILL_OP_CHANGE_ALL;

    event.type = RFKILL_TYPE_BLUETOOTH;

    event.soft = block;

    len = write(fd, &event, sizeof(event));

    if (len < 0){
        return QString("Failed to change RFKILL state");
    }

    close(fd);
    return block ? QString("blocked") : QString("unblocked");
}

int ClassRealize::getCurrentFlightMode(){

    struct rfkill_event event;
    ssize_t len;
    int fd;
    int bls = 0, unbls = 0;

    QList<int> status;

    fd = open("/dev/rfkill", O_RDONLY);
    if (fd < 0) {
        qCritical("Can't open RFKILL control device");
        return -1;
    }

    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        qCritical("Can't set RFKILL control device to non-blocking");
        close(fd);
        return -1;
    }

    while (1) {
        len = read(fd, &event, sizeof(event));

        if (len < 0) {
            if (errno == EAGAIN)
                break;
            qWarning("Reading of RFKILL events failed");
            break;
        }

        if (len != RFKILL_EVENT_SIZE_V1) {
            qWarning("Wrong size of RFKILL event\n");
            continue;
        }

//        printf("%u: %u\n", event.idx, event.soft);
        status.append(event.soft ? 1 : 0);

    }

    close(fd);

    if (status.length() == 0){
        return -1;
    }

    for (int s : status){
        s ? bls++ : unbls++;
    }

    if (bls == status.length()){ //block
        return 1;
    } else if (unbls == status.length()){ //unblock
        return 0;
    } else { //not block & not unblock
        return 0;
    }
}

QString ClassRealize::toggleFlightMode(bool enable){

    struct rfkill_event event;
    int fd;
    ssize_t len;

    __u8 block;

    fd = open("/dev/rfkill", O_RDWR);
    if (fd < 0) {
        return QString("Can't open RFKILL control device");
    }

    block = enable ? 1 : 0;

    memset(&event, 0, sizeof(event));

    event.op= RFKILL_OP_CHANGE_ALL;

    /* RFKILL_TYPE_ALL = 0 */
    event.type = RFKILL_TYPE_ALL;

    event.soft = block;

    len = write(fd, &event, sizeof(event));

    if (len < 0){
        return QString("Failed to change RFKILL state");
    }

    close(fd);
    return block ? QString("blocked") : QString("unblocked");
}

int ClassRealize::setCameraKeyboardLight(bool lightup){
    char * target = "/sys/class/leds/platform::cameramute/brightness";
    if (access(target, F_OK) == -1){
        return -1;
    }

    int value = lightup ? 1 : 0;

    char cmd[256];
    sprintf(cmd, "echo %d > %s", value, target);

    system(cmd);

    return 1;
}

int ClassRealize::setAirplaneModeKeyboardLight(bool lightup){
    char * target = "/sys/class/leds/platform::wlanmute/brightness";
    if (access(target, F_OK) == -1){
        return -1;
    }

    int value = lightup ? 1 : 0;

    char cmd[256];
    sprintf(cmd, "echo %d > %s", value, target);

    system(cmd);

    return 1;
}
