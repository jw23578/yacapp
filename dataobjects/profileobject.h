#ifndef PROFILEOBJECT_H
#define PROFILEOBJECT_H

#include "dataobjectinterface.h"

class ProfileObject : public DataObjectInterface
{
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, name, Name, "");
    YACAPPPROPERTY(QString, surname, Surame, "");
public:
    explicit ProfileObject(QObject *parent = nullptr);
};

#endif // PROFILEOBJECT_H
