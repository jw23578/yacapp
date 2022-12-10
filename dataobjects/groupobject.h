#ifndef GROUPOBJECT_H
#define GROUPOBJECT_H

#include "dataobjectinterface.h"

class GroupObject : public DataObjectInterface
{
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, name, Name, "");
public:
    explicit GroupObject(QObject *parent = nullptr);
};

#endif // GROUPOBJECT_H
