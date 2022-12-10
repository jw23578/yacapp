#ifndef MESSAGEOBJECT_H
#define MESSAGEOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>

class MessageObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, senderId, SenderId, "");
    YACAPPPROPERTY(QString, receiverId, ReceiverId, "");
    YACAPPPROPERTY(QDateTime, sent, Sent, QDateTime());
    YACAPPPROPERTY(QString, content, Content, "");
public:
    explicit MessageObject(const QString &senderId,
                           const QString &receiverId,
                           const QDateTime &sent,
                           const QString &content);

signals:

};

#endif // MESSAGEOBJECT_H
