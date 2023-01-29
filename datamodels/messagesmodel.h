#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include "templateddatamodel.h"
#include "dataobjects/messageobject.h"

class MessagesModel : public TemplatedDataModel<MessageObject>
{
    Q_OBJECT
    YACAPPPROPERTY(QString, profileId, ProfileID, "");
protected:
    enum MMRoles {
        DaySwitchRole = DefaultObjectRole + 1,
        NextSameTime,
        PrevSameTime,
        NextQMLWidth,
        PrevQMLWidth
    };
    QHash<int, QByteArray> customRoleNames() const override;
    QVariant customData(int row, int role) const override;
    bool sameTime(const MessageObject &a,
                  const MessageObject &b) const;

public:
    MessagesModel(QQmlApplicationEngine &engine);

    Q_INVOKABLE MessageObject *at(int index) const;

};

#endif // MESSAGESMODEL_H
