#include "rightgroupobject.h"
#include <QJsonObject>

RightGroupObject::RightGroupObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void RightGroupObject::fromJSON(QJsonObject &o)
{
    setId(o[tableFields.id].toString());
    setName(o[tableFields.name].toString());
    setAutomatic(o[tableFields.automatic].toString() == "t");
    setaccess_code(o[tableFields.access_code].toString());
    setcreater_id(o[tableFields.creater_id].toString());
}

void RightGroupObject::assign(const RightGroupObject &other)
{
    setId(other.id());
    setName(other.name());
    setAutomatic(other.automatic());
    setaccess_code(other.access_code());
    setcreater_id(other.creater_id());
}
