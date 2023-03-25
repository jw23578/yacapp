#ifndef RIGHTGROUPOBJECT_H
#define RIGHTGROUPOBJECT_H

#include "dataobjectinterface.h"
#include "yacAppAndServer/tablefields.h"

class RightGroupObject : public DataObjectInterface
{
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, creater_id, creater_id, "");
    YACAPPPROPERTY(QString, name, Name, "");
    YACAPPPROPERTY(QString, access_code, access_code, "");
    YACAPPPROPERTY(bool, automatic, Automatic, false);
public:
    explicit RightGroupObject(QObject *parent = nullptr);
    void fromJSON(QJsonObject &o);
    void assign(const RightGroupObject &other);
};

#endif // RIGHTGROUPOBJECT_H
