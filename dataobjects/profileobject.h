#ifndef PROFILEOBJECT_H
#define PROFILEOBJECT_H

#include "dataobjectinterface.h"

class ProfileObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, visibleName, VisibleName, "");
    YACAPPPROPERTY(int, unreadMessages, UnreadMessages, 0);
public:
    explicit ProfileObject(QObject *parent = nullptr);

    ProfileObject(const ProfileObject &other);
};

#endif // PROFILEOBJECT_H
