#ifndef PROFILEOBJECT_H
#define PROFILEOBJECT_H

#include "dataobjectinterface.h"
#include <QDateTime>
#include <QColor>

class ProfileObject : public DataObjectInterface
{
    Q_OBJECT
    YACAPPPROPERTY(QString, id, Id, "");
    YACAPPPROPERTY(QString, visibleName, VisibleName, "");
    YACAPPPROPERTY(int, unreadMessages, UnreadMessages, 0);
    YACAPPPROPERTY(QDateTime, lastAddedMessage, LastAddedMessage, QDateTime());
    YACAPPPROPERTY(QString, profileImageId, ProfileImageId, "");
    YACAPPPROPERTY(QString, public_key_base64, Public_key_base64, "");
    YACAPPPROPERTY(QColor, color, Color, "orange");
public:
    explicit ProfileObject(QObject *parent = nullptr);

    ProfileObject(const ProfileObject &other);

    void incUnreadMessages();

    void fromJSON(const QJsonObject &object);
    void assign(const ProfileObject &other);
};

#endif // PROFILEOBJECT_H
