#ifndef PROFILEOBJECT_H
#define PROFILEOBJECT_H

#include "dataobjectinterface.h"

class ProfileObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, visibleName, VisibleName, "");
public:
    explicit ProfileObject(QObject *parent = nullptr);
};

#endif // PROFILEOBJECT_H
