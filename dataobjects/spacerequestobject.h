#ifndef SPACEREQUESTOBJECT_H
#define SPACEREQUESTOBJECT_H

#include "dataobjectinterface.h"
#include "yacAppAndServer/tablefields.h"

class SpaceRequestObject : public DataObjectInterface
{
    Q_OBJECT
    TableFields tableFields;

    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, space_id, Space_id, "");
    YACAPPPROPERTY(QString, name, Name, "");
    YACAPPPROPERTY(QString, appuser_id, Appuser_id, "");
public:
    explicit SpaceRequestObject(QObject *parent = nullptr);

    void fromJSON(const QJsonObject &o);
    void assign(const SpaceRequestObject &other);
};

#endif // SPACEREQUESTOBJECT_H
