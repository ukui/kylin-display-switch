#ifndef MAPPINGTABLE_H
#define MAPPINGTABLE_H

#include <QObject>
#include <QMetaEnum>

enum {
    CAMERAKEY = 212,
    FLIGHTKEY = 238,
    MICROPHONEKEY = 248,
    SCREENKEY = 431,
    TOUCHPADKEY = 530,
    TOUCHPADONKEY = 531,
    TOUCHPADOFFKEY = 532,
    SCREENLOCKKEY = 689,
};

class MappingTable : public QObject
{
    Q_OBJECT
public:
    explicit MappingTable(QObject *parent = nullptr);

public:
    QString keyCodeToString(int code);

public:
    QMetaEnum metaTipIcon;


    enum MTIKey{
        CapslockOn, // 0
        CapslockOff,
        NumlockOn, // 2
        NumlockOff,
        TouchpadOn, // 4
        TouchpadOff,
        MicrophoneOn, // 6
        MicrophoneOff,
        CameraOn, // 8
        CameraOff,
        FlightOn, // 10
        FlightOff,
        WlanOn, // 12
        WlanOff,
        HandleKeys,
    };
    Q_ENUM(MTIKey)

};

#endif // MAPPINGTABLE_H
