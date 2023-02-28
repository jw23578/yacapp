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
}
