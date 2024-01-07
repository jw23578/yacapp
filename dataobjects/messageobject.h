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
    YACAPPPROPERTY(QDateTime, deleted_datetime, Deleted_datetime, QDateTime());
    YACAPPPROPERTY(QString, content, Content, "");
    YACAPPPROPERTY(bool, read, Read, false);
    YACAPPPROPERTY(double, qmlWidth, QmlWidth, 0);

    QString vecToBase64(const std::vector<unsigned char> &vec) const;
    void base64ToVec(const QByteArray &base64, std::vector<unsigned char> &vec);
    void decrypt(QString const &data, QString const &privateKey);

public:
    MessageObject(){}
    explicit MessageObject(const QString &id,
                           const QString &senderId,
                           const QString &receiverId,
                           const QDateTime &sent,
                           const QDateTime &received,
                           const QDateTime &deleted_datetime,
                           const QString &content,
                           const bool read,
                           QString const &privateKey);
    
    QString encryptToBase64(QString const &receiverPublicKey);

    void assign(const MessageObject &other);

signals:

};

#endif // MESSAGEOBJECT_H
