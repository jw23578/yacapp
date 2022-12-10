#include "messagesmodel.h"

QHash<int, QByteArray> MessagesModel::customRoleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DaySwitchRole] = "daySwitch";
    return roles;

}

QVariant MessagesModel::customData(int row, int role) const
{
    if (role == DaySwitchRole)
    {
        if (row == 0)
        {
            return true;
        }
        return getObject(row)->sent().date() != previousObject(row)->sent().date();
    }
    return QVariant();
}

MessagesModel::MessagesModel(QQmlApplicationEngine &engine):
    TemplatedDataModel<MessageObject>(engine, "MessagesModel", "message", TemplatedDataModel<MessageObject>::reverse)
{

}
