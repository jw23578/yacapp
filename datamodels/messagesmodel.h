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
        DaySwitchRole = DefaultObjectRole + 1
    };
    QHash<int, QByteArray> customRoleNames() const override;
    QVariant customData(int row, int role) const override;

public:
    MessagesModel(QQmlApplicationEngine &engine);

};

#endif // MESSAGESMODEL_H
