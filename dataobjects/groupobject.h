#ifndef GROUPOBJECT_H
#define GROUPOBJECT_H

#include "dataobjectinterface.h"
#include "yacAppAndServer/tablefields.h"

class GroupObject : public DataObjectInterface
{
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, name, Name, "");
public:
    explicit GroupObject(QObject *parent = nullptr);

    void fromJSON(QJsonObject &o);
};

#endif // GROUPOBJECT_H
