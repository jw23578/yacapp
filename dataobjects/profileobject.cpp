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
}
