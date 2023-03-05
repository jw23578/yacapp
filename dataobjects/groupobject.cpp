#include "groupobject.h"
#include <QJsonObject>

GroupObject::GroupObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void GroupObject::fromJSON(QJsonObject &o)
{
    setId(o[tableFields.id].toString());
    setName(o[tableFields.name].toString());
    setAutomatic(o[tableFields.automatic].toString() == "t");
    setaccess_code(o[tableFields.access_code].toString());
    setmember(o["member"].toString() == "t");
    setrequested(o["requested"].toString() == "t");
    setdenied(o["denied"].toString() == "t");
}
