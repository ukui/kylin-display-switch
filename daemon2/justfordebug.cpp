#include "justfordebug.h"


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

JustForDebug::JustForDebug(QObject *parent) : QObject(parent)
{

}

void JustForDebug::generalFun(){

}

int JustForDebug::generalFun2(){
    return 0;
}

QString JustForDebug::generalFun3(bool enable){

    return QString();

}
