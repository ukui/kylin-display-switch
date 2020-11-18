#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QThread>

#include "keymonitorthread.h"

class Daemon : public QObject
{
    Q_OBJECT

public:
    explicit Daemon();
    ~Daemon();


public:
    void begin();

private:
    QThread * thrd;
    KeyMonitorThread * kmt;
};

#endif // DAEMON_H
