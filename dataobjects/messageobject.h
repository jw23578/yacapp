#ifndef MESSAGEOBJECT_H
#define MESSAGEOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>

class MessageObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, senderId, SenderId, "");
    YACAPPPROPERTY(QString, receiverId, ReceiverId, "");
    YACAPPPROPERTY(QDateTime, sent, Sent, QDateTime());
    YACAPPPROPERTY(QDateTime, received, Received, QDateTime());
    YACAPPPROPERTY(QString, content, Content, "");
    YACAPPPROPERTY(bool, read, Read, false);
    YACAPPPROPERTY(double, qmlWidth, QmlWidth, 0);
public:
    MessageObject(){}
    explicit MessageObject(const QString &id,
                           const QString &senderId,
                           const QString &receiverId,
                           const QDateTime &sent,
                           const QDateTime &received,
                           const QString &content,
                           const bool read);

    QString base64();

    void assign(const MessageObject &other);

signals:

};

#endif // MESSAGEOBJECT_H
