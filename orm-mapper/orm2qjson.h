#ifndef ORM2QJSON_H
#define ORM2QJSON_H

#include <QJsonObject>
#include <QJsonArray>
#include "orm_implementions/yacbaseobject.h"
#include "orm_implementions/yacormfactory.h"

class ORM2QJson
{
public:
    ORM2QJson();

    void toJson(const YACBaseObject &object,
                QJsonObject &target);

    YACBaseObject *fromJson(const QJsonObject &source,
                            const YACORMFactory &factory);

    bool fromJson(const QJsonObject &source,
                  YACBaseObject &object);

    void toJson(const std::set<YACBaseObject *> &objects,
                QJsonArray &array);

    size_t fromJson(const QJsonArray &array,
                    const YACORMFactory &factory,
                    std::set<YACBaseObject*> &objects);
};

#endif // ORM2QJSON_H
