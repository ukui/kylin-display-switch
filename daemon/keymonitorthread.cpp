#include "keymonitorthread.h"

#include <X11/Xlibint.h>

KeyMonitorThread::KeyMonitorThread(QObject *parent) :
    QThread(parent)
{
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

//    // Receive from ALL clients, including future clients.
//    XRecordClientSpec clients = XRecordAllClients;
//    XRecordRange* range = XRecordAllocRange();
//    if (range == 0) {
//        fprintf(stderr, "unable to allocate XRecordRange\n");
//        return;
//    }

//    // Receive KeyPress, KeyRelease, ButtonPress, ButtonRelease and MotionNotify events.
//    memset(range, 0, sizeof(XRecordRange));
//    range->device_events.first = KeyPress;
//    range->device_events.last  = MotionNotify;

//    // And create the XRECORD context.
//    XRecordContext context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
//    if (context == 0) {
//        fprintf(stderr, "XRecordCreateContext failed\n");
//        return;
//    }
//    XFree(range);

//    XSync(display, True);

//    Display* display_datalink = XOpenDisplay(0);
//    if (display_datalink == 0) {
//        fprintf(stderr, "unable to open second display\n");
//        return;
//    }

//    if (!XRecordEnableContext(display_datalink, context,  callback, (XPointer) this)) {
//        fprintf(stderr, "XRecordEnableContext() failed\n");
//        return;
//    }
}

void KeyMonitorThread::callback(XPointer ptr, XRecordInterceptData* data)
{
    ((KeyMonitorThread *) ptr)->handleRecordEvent(data);
}

void KeyMonitorThread::handleRecordEvent(XRecordInterceptData* data)
{
    if (data->category == XRecordFromServer) {
        xEvent * event = (xEvent *)data->data;
        switch (event->u.u.type) {
//        case ButtonPress:
//            if (filterWheelEvent(event->u.u.detail)) {
//                isPress = true;
//                emit buttonPress(
//                    event->u.keyButtonPointer.rootX,
//                    event->u.keyButtonPointer.rootY);
//            }

//            break;
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
            emit keyPress(((unsigned char*) data->data)[1]);

            break;
        case KeyRelease:
            emit keyRelease(((unsigned char*) data->data)[1]);

            break;
        default:
            break;
        }
    }

    fflush(stdout);
    XRecordFreeData(data);
}