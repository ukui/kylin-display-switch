#include "mappingtable.h"

MappingTable::MappingTable(QObject *parent) : QObject(parent)
{
    metaTipIcon = QMetaEnum::fromType<MappingTable::MTIKey>();

}

QString MappingTable::keyCodeToString(int code){
    return metaTipIcon.valueToKey(code);    //未匹配到则返回空
}
