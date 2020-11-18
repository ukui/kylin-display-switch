#ifndef KEYMONITORTHREAD_H
#define KEYMONITORTHREAD_H

#include <QThread>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>


class KeyMonitorThread : public QThread
{
    Q_OBJECT

public:
    explicit KeyMonitorThread(QObject *parent = 0);
    ~KeyMonitorThread();

signals:
    void keyPress(int code);
    void keyRelease(int code);
    void jobComplete();

protected:
    void run();
    static void callback(XPointer trash, XRecordInterceptData* data);
    void handleRecordEvent(XRecordInterceptData *);

};

#endif // KEYMONITORTHREAD_H
