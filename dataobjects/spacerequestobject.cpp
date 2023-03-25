#include "spacerequestobject.h"
#include <QJsonObject>

SpaceRequestObject::SpaceRequestObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void SpaceRequestObject::fromJSON(const QJsonObject &o)
{
    setId(o[tableFields.id].toString());
    setName(o[tableFields.name].toString());
    setSpace_id(o[tableFields.space_id].toString());
    setAppuser_id(o[tableFields.appuser_id].toString());
}

void SpaceRequestObject::assign(const SpaceRequestObject &other)
{
    setId(other.id());
    setName(other.name());
    setSpace_id(other.space_id());
    setAppuser_id(other.appuser_id());
}
