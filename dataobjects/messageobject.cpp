#include "messageobject.h".h"

MessageObject::MessageObject(const QString &senderId,
                             const QString &receiverId,
                             const QDateTime &sent,
                             const QString &content)
    : DataObjectInterface{0}
{
    setSenderId(senderId);
    setReceiverId(receiverId);
    setSent(sent);
    setContent(content);
}
