#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "dataobjectinterface.h"
#include "yacAppAndServer/tablefields.h"

class SpaceObject : public DataObjectInterface
{
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, creater_id, creater_id, "");
    YACAPPPROPERTY(QString, name, Name, "");
    YACAPPPROPERTY(QString, access_code, access_code, "");
    YACAPPPROPERTY(bool, automatic, Automatic, false);
    YACAPPPROPERTY(bool, member, member, false);
    YACAPPPROPERTY(bool, requested, requested, false);
    YACAPPPROPERTY(bool, denied, denied, false);
public:
    explicit SpaceObject(QObject *parent = nullptr);
    void fromJSON(QJsonObject &o);
};

#endif // SPACEOBJECT_H
