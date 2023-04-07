#include "orm2qjson.h"

ORM2QJson::ORM2QJson()
{

}

void ORM2QJson::toJson(const YACBaseObject &object, QJsonObject &target)
{
    target["ORMName"] = object.getORMName();
    for (const auto &pn : object.propertyNames())
    {
        if (object.propertyIsNull(pn))
        {
            target[pn] = QJsonValue::Null;
        }
        else
        {
            target[pn] = object.getPropertyToString(pn);
        }
    }
}

YACBaseObject *ORM2QJson::fromJson(const QJsonObject &source,
                                   const YACORMFactory &factory)
{
    if (!source.contains("ORMName"))
    {
        return 0;
    }
    YACBaseObject *object(factory.create(source["ORMName"].toString()));
    if (!object)
    {
        return 0;
    }
    fromJson(source, *object);
    return object;

}

bool ORM2QJson::fromJson(const QJsonObject &source,
                         YACBaseObject &object)
{
    if (!source.contains("ORMName"))
    {
        return false;
    }
    if (object.getORMName() != source["ORMName"].toString())
    {
        return false;
    }
    for (const auto &pn : object.propertyNames())
    {
        if (!source.contains(pn) || source[pn].isNull())
        {
            object.setPropertyNull(pn, true);
        }
        else
        {
            object.setPropertyFromString(pn, source[pn].toString());
        }
    }
    return true;
}
