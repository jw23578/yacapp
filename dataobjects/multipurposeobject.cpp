#include "multipurposeobject.h"

MultiPurposeObject::MultiPurposeObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

void MultiPurposeObject::assign(const MultiPurposeObject &other)
{
    setId(other.id());
    setName(other.name());
    setCaption(other.caption());
}
