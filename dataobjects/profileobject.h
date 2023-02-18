#ifndef PROFILEOBJECT_H
#define PROFILEOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>

class ProfileObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, visibleName, VisibleName, "");
    YACAPPPROPERTY(int, unreadMessages, UnreadMessages, 0);
    YACAPPPROPERTY(QDateTime, lastAddedMessage, LastAddedMessage, QDateTime());
    YACAPPPROPERTY(QString, profileImageId, ProfileImageId, "");
public:
    explicit ProfileObject(QObject *parent = nullptr);

    ProfileObject(const ProfileObject &other);

    void incUnreadMessages();
};

#endif // PROFILEOBJECT_H
