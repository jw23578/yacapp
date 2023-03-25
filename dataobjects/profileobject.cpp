#include "profileobject.h"

ProfileObject::ProfileObject(QObject *parent)
    : DataObjectInterface{parent}
{

}

ProfileObject::ProfileObject(const ProfileObject &other):DataObjectInterface(0)
{
    setId(other.id());
    setVisibleName(other.visibleName());
    setUnreadMessages(other.unreadMessages());
    setProfileImageId(other.profileImageId());
}

void ProfileObject::incUnreadMessages()
{
    setUnreadMessages(unreadMessages() + 1);
    setLastAddedMessage(QDateTime::currentDateTime());
}

void ProfileObject::assign(const ProfileObject &other)
{
    setId(other.id());
    setVisibleName(other.visibleName());
    setUnreadMessages(other.unreadMessages());
    setLastAddedMessage(other.lastAddedMessage());
    setProfileImageId(other.profileImageId());
}
