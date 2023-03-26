#include "newsobject.h"

NewsObject::NewsObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void NewsObject::assign(const NewsObject &other)
{
    setId(other.id());
    setType(other.type());
    setCaption(other.caption());
    setSpaceId(other.spaceId());
    setSpaceRequesterId(other.spaceRequesterId());
}
