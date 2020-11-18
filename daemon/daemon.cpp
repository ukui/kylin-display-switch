#include "daemon.h"

#include <QDebug>

Daemon::Daemon()
{

}

Daemon::~Daemon()
{
}

void Daemon::begin(){
//    thrd = new QThread;
//    kmt = new KeyMonitorThread;

//    connect(kmt, &KeyMonitorThread::keyPress, this, [=](int code){
//        qDebug() << "key press:" << code;
//    }, Qt::QueuedConnection);

//    connect(kmt, &KeyMonitorThread::jobComplete, this, [=]{
//        thrd->quit(); //退出事件循环
//        thrd->wait(); //释放资源
//    });

//    kmt->moveToThread(thrd);

//    connect(thrd, &QThread::started, kmt, &KeyMonitorThread::run);

//    connect(thrd, &QThread::finished, kmt, &KeyMonitorThread::deleteLater);

//    thrd->start();
}
