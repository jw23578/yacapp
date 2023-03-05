#include "spaceobject.h"
#include <QJsonObject>

SpaceObject::SpaceObject(QObject *parent)
    : DataObjectInterface{parent}
{

}


void SpaceObject::fromJSON(QJsonObject &o)
{
    setId(o[tableFields.id].toString());
    setName(o[tableFields.name].toString());
    setAutomatic(o[tableFields.automatic].toString() == "t");
    setaccess_code(o[tableFields.access_code].toString());
    setcreater_id(o[tableFields.creater_id].toString());
    setmember(o["member"].toString() == "t");
    setrequested(o["requested"].toString() == "t");
    setdenied(o["denied"].toString() == "t");
}
