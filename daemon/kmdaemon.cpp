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
#include <xkbcommon/xkbcommon-keysyms.h>
#include <QFileInfo>
#include <QProcess>

#include <X11/extensions/XInput.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>


#define UKCCOSD_SCHEMA "org.ukui.control-center.osd"
#define KYCCOSD_SCHEMA "org.kylin.control-center.osd"
#define KDSOSD_SCHEMA "org.ukui.kds.osd"

#define UK_TOUCHPAD_SCHEMA "org.ukui.peripherals-touchpad"
#define KY_TOUCHPAD_SCHEMA "org.mate.peripherals-touchpad"

#define UK_KEYBOARD_SCHEMA "org.ukui.peripherals-keyboard"

#define UK_POWERMANAGER_SCHEMA "org.ukui.power-manager"
#define KY_POWERMANAGER_SCHEMA "org.mate.power-manager"

#define SHOW_TIP_KEY "show-lock-tip"
#define RUNNING_KEY "running"
#define TP_ENABLE_KEY "touchpad-enabled"
#define MP_ENABLE_KEY "microphone"

#define CAPSLOCK_STATUS_KEY "capslock-state"
#define NUMLOCK_STATUS_KEY "numlock-state"

#define BRIGHTNESS_KEY "brightness-ac"

#define SCREENOPENVALUE 99
#define SCREENCLOSEVALUE 0

Display * display;

KMDaemon::KMDaemon()
{
    thrd = new QThread;
    kmt = new KeyMonitorThread;
    iface = new QDBusInterface("org.ukui.kds", \
                               "/", \
                               "org.ukui.kds.interface", \
                               QDBusConnection::systemBus());

    ifaceScreenSaver = new QDBusInterface("org.ukui.ScreenSaver", \
                                          "/", \
                                          "org.ukui.ScreenSaver", \
                                          QDBusConnection::sessionBus());

    modifyKeyPressed = false;
    stInstalled = true;
    kbInstalled = false;

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

    //X data init
    display = XOpenDisplay(0);


    const QByteArray id2(UK_KEYBOARD_SCHEMA);
    if (QGSettings::isSchemaInstalled(id2)){

        kbInstalled = true;

        kbGSettings = new QGSettings(id2);

        connect(kbGSettings, &QGSettings::changed, this, [=](QString key){

            if (stInstalled && !settings->get(SHOW_TIP_KEY).toBool()){
                qWarning("MediaKey Tip is Closed\n");
                return;
            }

            if (ifaceScreenSaver->isValid()){
                QDBusReply<bool>reply = ifaceScreenSaver->call("GetLockState");
                if (reply.isValid()){
                    if (reply.value()){
                        qWarning("MediaKey Tip is Closed because ScreenLock\n");
                        return;
                    }
                }
            }

            if (QString::compare(key, "capslockState") == 0){

                bool current = kbGSettings->get(CAPSLOCK_STATUS_KEY).toBool();

                if (current){
                    iface->call("emitShowTipsSignal", MappingTable::CapslockOn);
                } else {
                    iface->call("emitShowTipsSignal", MappingTable::CapslockOff);
                }

            } else if (QString::compare(key, "numlockState") == 0){

                bool current = QString::compare(kbGSettings->get(NUMLOCK_STATUS_KEY).toString(), QString("on")) == 0;

                if (current){
                    iface->call("emitShowTipsSignal", MappingTable::NumlockOn);
                } else {
                    iface->call("emitShowTipsSignal", MappingTable::NumlockOff);
                }

            }
        });

    } else {
        capslockStatus = false;
        numlockStatus = false;
    }

    QDBusReply<int> reply = iface->call("getCameraDeviceEnable");
    if (reply.isValid()){
        int current = reply.value();

        if (current == -1)
            cameraEnableStatus = true;
        else{
            cameraEnableStatus = current ? true : false;
        }

    }

    keyboardLightInit();

    const QByteArray idtp(UK_TOUCHPAD_SCHEMA);

    //KeyCode 比 正常键值大 8，原因未知
    connect(kmt, &KeyMonitorThread::keyPress, this, [=](KeySym mks, KeyCode mkc){
        Q_UNUSED(mkc)
//        qDebug() << "key press:" << mkc - 8;

//        if (!iface->isValid()){
//            qCritical() << "Create Client Interface Failed When execute chage: " << QDBusConnection::systemBus().lastError();
//            return;
//        }

        if (mks == XKB_KEY_XF86TouchpadOn){
            //更新gsettings
            if (QGSettings::isSchemaInstalled(idtp)){
                QGSettings * st = new QGSettings(idtp);
                st->set(TP_ENABLE_KEY, true);
                delete st;
            }
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOn);
        } else if (mks == XKB_KEY_XF86TouchpadOff){
            //更新gsettings
            if (QGSettings::isSchemaInstalled(idtp)){
                QGSettings * st = new QGSettings(idtp);
                st->set(TP_ENABLE_KEY, false);
                delete st;
            }
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOff);
        } else if (mks == XKB_KEY_XF86RFKill){
            QDBusReply<int> reply = iface->call("getCurrentFlightMode");
            if (reply.isValid()){
                int current = reply.value();

                if (current != -1){
                    if (current){
                        iface->call("emitShowTipsSignal", MappingTable::FlightOn);
                    } else {
                        iface->call("emitShowTipsSignal", MappingTable::FlightOff);
                    }
                }

            }
        } else if (mks == XKB_KEY_XF86WebCam){

            cameraEnableStatus = !cameraEnableStatus;

            if (cameraEnableStatus)
                iface->call("emitShowTipsSignal", MappingTable::CameraOn);
            else
                iface->call("emitShowTipsSignal", MappingTable::CameraOff);

        } else if (mks == XKB_KEY_XF86AudioMicMute){
//            qDebug() << "mic mute";
        } else if (mks == XKB_KEY_XF86WLAN){
            wlanToggle();
        }

        if (mks == XK_Super_L || mks== XK_Super_R){
            modifyKeyPressed = true;
        } else if (mks == XK_p){
            if (modifyKeyPressed){
//                qDebug("Win + P Pressed!\n");
//                QProcess process;
//                QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//                env.insert("DISPLAY", ":0");
//                process.setProcessEnvironment(env);
//                process.startDetached("/usr/bin/kydisplayswitch");

            } else {
                iface->call("emitCloseApp");
            }

        } else if (mks == XKB_KEY_XF86Display){
            /* Do Nothing, Just Not CloseApp*/

        }else if (mks == XK_KP_Enter || mks == XK_Return){
            iface->call("emitMakeClicked");

        } else if (mks == XK_Up || mks == XK_KP_Up) {
            iface->call("emitLastOption");

        } else if (mks == XK_Down || mks == XK_KP_Down) {
            iface->call("emitNextOption");

        } else if (mks == XK_Caps_Lock){

            capslockStatus = !capslockStatus;

            if (stInstalled && !settings->get(SHOW_TIP_KEY).toBool()){
                qWarning("MediaKey Tip is Closed\n");
                return;
            }

            if (ifaceScreenSaver->isValid()){
                QDBusReply<bool>reply = ifaceScreenSaver->call("GetLockState");
                if (reply.isValid()){
                    if (reply.value()){
                        qWarning("MediaKey Tip is Closed because ScreenLock\n");
                        return;
                    }
                }
            }

            /* usd的键盘GSettings已安装，使用gsettings来触发，这里直接返回 */
            if (kbInstalled)
                return;

            if (capslockStatus){
                iface->call("emitShowTipsSignal", MappingTable::CapslockOn);
            } else {
                iface->call("emitShowTipsSignal", MappingTable::CapslockOff);
            }

        } else if (mks == XK_Num_Lock){

            numlockStatus = !numlockStatus;

            if (stInstalled && !settings->get(SHOW_TIP_KEY).toBool()){
                qWarning("MediaKey Tip is Closed\n");
                return;
            }

            if (ifaceScreenSaver->isValid()){
                QDBusReply<bool>reply = ifaceScreenSaver->call("GetLockState");
                if (reply.isValid()){
                    if (reply.value()){
                        qWarning("MediaKey Tip is Closed because ScreenLock\n");
                        return;
                    }
                }
            }

            /* usd的键盘GSettings已安装，使用gsettings来触发，这里直接返回 */
            if (kbInstalled)
                return;

            if (numlockStatus){
                iface->call("emitShowTipsSignal", MappingTable::NumlockOn);
            } else {
                iface->call("emitShowTipsSignal", MappingTable::NumlockOff);
            }


        } else {
            iface->call("emitCloseApp");
        }

    }, Qt::QueuedConnection);

    connect(kmt, &KeyMonitorThread::keyRelease, this, [=](KeySym mks, KeyCode mkc){
        Q_UNUSED(mkc)
//        qDebug() << "key release:" << mkc - 8;
        if (mks == XK_Super_L || mks== XK_Super_R){
            modifyKeyPressed = false;
        }
    }, Qt::QueuedConnection);


    connect(kmt, &KeyMonitorThread::buttonPress, this, [=] (int x, int y) {
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

    delete ifaceScreenSaver;

    if (stInstalled)
        delete settings;

    if (kbInstalled)
        delete kbGSettings;
}

void KMDaemon::begin(){

    kmt->moveToThread(thrd);

    thrd->start();
}

void KMDaemon::keyboardLightInit(){

    airplanModeKeyLightInit();

    disableCameraKeyLightInit();

}

void KMDaemon::airplanModeKeyLightInit(){
    QDBusReply<int> reply = iface->call("getCurrentFlightMode");

    if (!reply.isValid()){
        return;
    }

    int current = reply.value();
    if (current == -1){
        return;
    }
    /* 设置飞行模式键盘灯 */
    QDBusReply<int> reply2 = iface->call("setAirplaneModeKeyboardLight", current);
    if (!reply2.isValid()){
        //                qWarning("Set AirplaneMode Keyboardlight Failed");
    }

}

void KMDaemon::disableCameraKeyLightInit(){
    QDBusReply<int> cReply = iface->call("getCameraDeviceEnable");
    if (!cReply.isValid()){
        return;
    }

    int result = cReply.value();

    if (result == -1){
        return;
    }
    /* 设置禁用摄像头键盘灯 */
    QDBusReply<int> cReply2 = iface->call("setCameraKeyboardLight", result ? false : true);
    if (!cReply2.isValid()){
        //                qWarning("Set Camera Keyboardlight Failed");
    }

}

/*
 * OBSOLETE
 */
bool KMDaemon::getCurrentCapslockStatus(){

    Bool state;


    static Atom CapsLock = XInternAtom(display, "Caps Lock", False);

    XkbGetNamedIndicator(display, CapsLock, NULL, &state, NULL, NULL);

    return state;
}

/*
 * OBSOLETE
 */
bool KMDaemon::getCurrentNumlockStatus(){

    Bool state;

    static Atom NumLock = XInternAtom(display, "Num Lock", False);

    XkbGetNamedIndicator(display, NumLock, NULL, &state, NULL, NULL);

    return state;
}

void KMDaemon::mediaKeyManager(int code){

    switch (code) {
    case TOUCHPADKEY:
        qDebug("Key Received %d!\n", TOUCHPADKEY);
        touchpadToggle();
        break;
    case TOUCHPADONKEY:
        qDebug("Key Received %d!\n", TOUCHPADONKEY);
        touchpadToggle2(true);
        break;
    case TOUCHPADOFFKEY:
        qDebug("Key Received %d!\n", TOUCHPADOFFKEY);
        touchpadToggle2(false);
        break;
    case MICROPHONEKEY:
        qDebug("Key Received %d!\n", MICROPHONEKEY);
        microphoneToggle();
        break;
    case CAMERAKEY:
        qDebug("Key Received %d!\n", CAMERAKEY);
        cameraToggle();
        break;
    case FLIGHTKEY:
        qDebug("Key Received %d!\n", FLIGHTKEY);
        flightToggle();
        break;
    case SCREENKEY:
        qDebug("Key Received %d!\n", SCREENKEY);
        screenToggle();
        break;
    case SCREENLOCKKEY:
        qDebug("Key Received %d!\n", SCREENLOCKKEY);
        screenLock();
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

    deviceinfos = XListInputDevices (display, &n_devices);

    if (deviceinfos == NULL){
        qCritical("XListInputDevices is NULL!\n");
        return;
    }

    for (int i = 0; i < n_devices; i++){
        XDevice * device;
        Atom realtype, prop;
        XDeviceInfo deviceinfo = deviceinfos[i];

        if (deviceinfo.type != XInternAtom (display, XI_TOUCHPAD, False)){
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

//            if (QString(deviceinfo.name).contains("USB Optical Mouse"))
//                continue;

            qDebug("Get Input name:%s; id: %d!\n", deviceinfo.name, (int)deviceinfo.id);
            if (nitems == 1){
//                data[0] = (data[0] == 0) ? 1 : 0;

                const QByteArray id(UK_TOUCHPAD_SCHEMA);

                if (data[0] == 1){
                    QString cmd = QString("xinput disable %1").arg(QString::number((int)deviceinfo.id));
                    qDebug("Disable Input Device %s\n", deviceinfo.name);
                    system(cmd.toLatin1().data());

                    //更新gsettings
                    if (QGSettings::isSchemaInstalled(id)){
                        QGSettings * st = new QGSettings(id);
                        st->set(TP_ENABLE_KEY, false);
                        delete st;
                    }

                    iface->call("emitShowTipsSignal", MappingTable::TouchpadOff);
                } else {
                    QString cmd = QString("xinput enable %1").arg(QString::number((int)deviceinfo.id));
                    qDebug("Enable Input Device %s\n", deviceinfo.name);
                    system(cmd.toLatin1().data());

                    //更新gsettings
                    if (QGSettings::isSchemaInstalled(id)){
                        QGSettings * st = new QGSettings(id);
                        st->set(TP_ENABLE_KEY, true);
                        delete st;
                    }

                    iface->call("emitShowTipsSignal", MappingTable::TouchpadOn);
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
            qDebug("Enable Touchpad By UK GSettings!\n");
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOn);
        } else {
            qDebug("Disable Touchpad By UK GSettings!\n");
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOff);
        }
        delete st;
    } else if (QGSettings::isSchemaInstalled(idd)){
        QGSettings * st = new QGSettings(idd);
        st->set(TP_ENABLE_KEY, enable);
        if (enable){
            qDebug("Enable Touchpad By KY GSettings!\n");
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOn);
        } else {
            qDebug("Disable Touchpad By KY GSettings!\n");
            iface->call("emitShowTipsSignal", MappingTable::TouchpadOff);
        }
        delete st;
    } else {
       qWarning("Touchpad GSettings is not install!");
    }
}

void KMDaemon::microphoneToggle(){
    const QByteArray id(KDSOSD_SCHEMA);
    if (QGSettings::isSchemaInstalled(id)){
        QGSettings * st = new QGSettings(id);
        bool current = st->get(MP_ENABLE_KEY).toBool();

        st->set(MP_ENABLE_KEY, !current);

        if (current){
            qDebug("Disable Microphone!\n");
            iface->call("emitShowTipsSignal", MappingTable::MicrophoneOff);
        } else {
            qDebug("Enable Microphone!\n");
            iface->call("emitShowTipsSignal", MappingTable::MicrophoneOn);
        }
    }
}

void KMDaemon::cameraToggle(){
    QDBusReply<QString> reply = iface->call("getCameraBusinfo");
    if (reply.isValid()){
        QString businfo = reply.value();
        QDBusReply<QString> reply2 = iface->call("toggleCameraDevice", businfo);

        if (reply2.isValid()){
            QString result = reply2.value();

            if (result == QString("binded")){
                qDebug("Enable Camera Device!\n");
                iface->call("setCameraKeyboardLight", false);
                iface->call("emitShowTipsSignal", MappingTable::CameraOn);
            } else if (result == QString("unbinded")){
                qDebug("Disable Camera Device!\n");
                iface->call("setCameraKeyboardLight", true);
                iface->call("emitShowTipsSignal", MappingTable::CameraOff);
            } else {
                qWarning("%s", result.toLatin1().data());
            }

        } else {
            qWarning("Toggle Camera device Failed!");
        }

    } else {
        qWarning("Get Camera Businfo Failed!");
    }
}

void KMDaemon::flightToggle(){

    QDBusReply<int> reply = iface->call("getCurrentFlightMode");
    if (reply.isValid()){
        int current = reply.value();

        if (current == -1){
            qWarning("Error Occur When Get Current FlightMode");
            return;
        }

        QDBusReply<QString> reply2 = iface->call("toggleFlightMode", !current);
        if (reply2.isValid()){
            QString result = reply2.value();
            if (result == QString("blocked")){
                qDebug("Enable Flight Mode!\n");
                iface->call("setAirplaneModeKeyboardLight", true);
                iface->call("emitShowTipsSignal", MappingTable::FlightOn);
            } else if (result == QString("unblocked")){
                qDebug("Disable Flight Mode!\n");
                iface->call("setAirplaneModeKeyboardLight", false);
                iface->call("emitShowTipsSignal", MappingTable::FlightOff);
            } else {
                qWarning("%s", result.toLatin1().data());
            }
        } else {
            qWarning("Toggle Flight Mode Failed!");
        }


    } else {
        qWarning("Get Current FlightMode Failed!");
    }
}

void KMDaemon::wlanToggle(){

    QDBusReply<int> reply = iface->call("getCurrentWlanMode");
    if (reply.isValid()){
        int current = reply.value();

        if (current == -1){
            qWarning("Error Occur When Get Current WlanMode");
            return;
        }

        bool status = current ? true : false;

        QDBusReply<QString> reply2 = iface->call("toggleWlanMode", !status);
        if (reply2.isValid()){
            QString result = reply2.value();
            if (result == QString("blocked")){
                qDebug("Disable Wlan Mode!\n");
                iface->call("emitShowTipsSignal", MappingTable::WlanOff);
            } else if (result == QString("unblocked")){
                qDebug("Enable Wlan Mode!\n");
                iface->call("emitShowTipsSignal", MappingTable::WlanOn);
            } else {
                qWarning("%s", result.toLatin1().data());
            }
        } else {
            qWarning("Toggle Wlan Mode Failed!");
        }
    }
}

void KMDaemon::screenLock(){
    QFileInfo lock1("/usr/bin/mate-screensaver-command");
    QFileInfo lock2("/usr/bin/ukui-screensaver-command");
    QString cmd;
    if (lock1.isExecutable()){
        cmd = QString("mate-screensaver-command --lock");
    } else if (lock2.isExecutable()){
        cmd = QString("ukui-screensaver-command -l");
    } else {
        cmd = QString("");
    }

    if (!cmd.isEmpty())
        system(cmd.toLatin1().data());
}

void KMDaemon::screenToggle(){
    const QByteArray id(UK_POWERMANAGER_SCHEMA);
    const QByteArray idd(KY_POWERMANAGER_SCHEMA);

    if (QGSettings::isSchemaInstalled(id)){
        QGSettings * st1 = new QGSettings(id);
        int value = st1->get(BRIGHTNESS_KEY).toInt();

        if (value == SCREENCLOSEVALUE){
            st1->set(BRIGHTNESS_KEY, SCREENOPENVALUE);
        } else {
            st1->set(BRIGHTNESS_KEY, SCREENCLOSEVALUE);
        }

        delete st1;
    } else if (QGSettings::isSchemaInstalled(idd)){
        QGSettings * st1 = new QGSettings(idd);
        int value = st1->get(BRIGHTNESS_KEY).toInt();

        if (value == SCREENCLOSEVALUE){
            st1->set(BRIGHTNESS_KEY, SCREENOPENVALUE);
        } else {
            st1->set(BRIGHTNESS_KEY, SCREENCLOSEVALUE);
        }

        delete st1;
    } else {
        qWarning("Touchpad GSettings is not install!");
    }
}
