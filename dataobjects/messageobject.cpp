#include "messageobject.h".h"

MessageObject::MessageObject(const QString &id,
                             const QString &senderId,
                             const QString &receiverId,
                             const QDateTime &sent,
                             const QDateTime &received,
                             const QString &content,
                             const bool read)
    : DataObjectInterface{0}
{
    setId(id);
    setSenderId(senderId);
    setReceiverId(receiverId);
    setSent(sent);
    setReceived(received);
    setContent(content);
    setRead(read);
}
