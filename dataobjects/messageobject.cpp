#include "messageobject.h"

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

QString MessageObject::base64()
{
    return QByteArray(content().toUtf8()).toBase64();
}

void MessageObject::assign(const MessageObject &other)
{
    setId(other.id());
    setSenderId(other.senderId());
    setReceiverId(other.receiverId());
    setSent(other.sent());
    setReceived(other.received());
    setContent(other.content());
    setRead(other.read());
}
