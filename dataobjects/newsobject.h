#ifndef NEWSOBJECT_H
#define NEWSOBJECT_H

#include "dataobjectinterface.h"

class NewsObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, type, Type, "");
    YACAPPPROPERTY(QString, caption, Caption, "");
    YACAPPPROPERTY(QString, spaceId, SpaceId, "");
    YACAPPPROPERTY(QString, spaceRequesterId, SpaceRequesterId, "");
public:
    explicit NewsObject(QObject *parent = nullptr);

    void assign(const NewsObject &other);
};

#endif // NEWSOBJECT_H
