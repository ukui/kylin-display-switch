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
#include "keymonitorthread.h"

#include <X11/Xlibint.h>

KeyMonitorThread::KeyMonitorThread(QObject *parent) :
    QThread(parent)
{
    display = XOpenDisplay(0);
    if (display == 0) {
        fprintf(stderr, "unable to open display\n");
        return;
    }

}

KeyMonitorThread::~KeyMonitorThread()
{
}


void KeyMonitorThread::run(){
//    Display* display = XOpenDisplay(0);
//    if (display == 0) {
//        fprintf(stderr, "unable to open display\n");
//        return;
//    }

    // Receive from ALL clients, including future clients.
    XRecordClientSpec clients = XRecordAllClients;
    XRecordRange* range = XRecordAllocRange();
    if (range == 0) {
        fprintf(stderr, "unable to allocate XRecordRange\n");
        return;
    }

    // Receive KeyPress, KeyRelease, ButtonPress, ButtonRelease and MotionNotify events.
    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyPress;
    range->device_events.last  = MotionNotify;

    // And create the XRECORD context.
    XRecordContext context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    if (context == 0) {
        fprintf(stderr, "XRecordCreateContext failed\n");
        return;
    }
    XFree(range);

    XSync(display, True);

    Display* display_datalink = XOpenDisplay(0);
    if (display_datalink == 0) {
        fprintf(stderr, "unable to open second display\n");
        return;
    }

    if (!XRecordEnableContext(display_datalink, context,  callback, (XPointer) this)) {
        fprintf(stderr, "XRecordEnableContext() failed\n");
        return;
    }

}

void KeyMonitorThread::callback(XPointer ptr, XRecordInterceptData* data)
{
    ((KeyMonitorThread *) ptr)->handleRecordEvent(data);
}

void KeyMonitorThread::handleRecordEvent(XRecordInterceptData* data)
{
    qDebug("handleRecordEvent");
    if (data->category == XRecordFromServer) {
        xEvent * event = (xEvent *)data->data;
        qDebug("category is XRecordFromServer");

        KeySym keySym = XKeycodeToKeysym(display, event->u.u.detail, 0);
        KeyCode keyCode = XKeysymToKeycode(display, keySym);

        switch (event->u.u.type) {
        case ButtonPress:
            if (filterWheelEvent(event->u.u.detail)) {
                emit buttonPress(
                            event->u.keyButtonPointer.rootX,
                            event->u.keyButtonPointer.rootY);
            }

            break;
//        case MotionNotify:
//            if (isPress) {
//                emit buttonDrag(
//                    event->u.keyButtonPointer.rootX,
//                    event->u.keyButtonPointer.rootY);
//            }

//            break;
//        case ButtonRelease:
//            if (filterWheelEvent(event->u.u.detail)) {
//                isPress = false;
//                emit buttonRelease(
//                    event->u.keyButtonPointer.rootX,
//                    event->u.keyButtonPointer.rootY);
//            }

//            break;
        case KeyPress:
//            emit keyPress(((unsigned char*) data->data)[1]);
            emit keyPress(keySym, keyCode);

            break;
        case KeyRelease:
//            emit keyRelease(((unsigned char*) data->data)[1]);
            emit keyRelease(keySym, keyCode);

            break;
        default:
            break;
        }
    }

    fflush(stdout);
    XRecordFreeData(data);
}

bool KeyMonitorThread::filterWheelEvent(int detail)
{
    return detail != WheelUp && detail != WheelDown && detail != WheelLeft && detail != WheelRight;
}



void KeyMonitorThread::callJobComplete(){
    emit jobComplete();
}
