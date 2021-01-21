#ifndef JUSTFORDEBUG_H
#define JUSTFORDEBUG_H

#include <QObject>

class JustForDebug : public QObject
{
    Q_OBJECT
public:
    explicit JustForDebug(QObject *parent = nullptr);

public:
    void generalFun();
    int generalFun2();
    QString generalFun3(bool enable);

signals:

};

#endif // JUSTFORDEBUG_H
