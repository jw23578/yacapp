#include "profileobject.h"
#include <QJsonObject>

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
    setColor(other.color());
    setMessageFontColor(other.messageFontColor());
}

void ProfileObject::incUnreadMessages()
{
    setUnreadMessages(unreadMessages() + 1);
    setLastAddedMessage(QDateTime::currentDateTime());
}

void ProfileObject::fromJSON(const QJsonObject &object)
{
    setId(object[tableFields.id].toString());
    setVisibleName(object[tableFields.visible_name].toString());
    setProfileImageId(object[tableFields.image_id].toString());
    if (object["color"].toString().size())
    {
        setColor(object["color"].toString());
    }
    if (object["messageFontColor"].toString().size())
    {
        setMessageFontColor(object["messageFontColor"].toString());
    }
}

void ProfileObject::assign(const ProfileObject &other)
{
    setId(other.id());
    setVisibleName(other.visibleName());
    setUnreadMessages(other.unreadMessages());
    setLastAddedMessage(other.lastAddedMessage());
    setProfileImageId(other.profileImageId());
    setColor(other.color());
    setMessageFontColor(other.messageFontColor());
}
