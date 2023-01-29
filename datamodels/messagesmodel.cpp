#include "messagesmodel.h"
#include <math.h>

QHash<int, QByteArray> MessagesModel::customRoleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DaySwitchRole] = "daySwitch";
    roles[NextSameTime] = "nextSameTime";
    roles[PrevSameTime] = "prevSameTime";
    roles[NextQMLWidth] = "nextQMLWidth";
    roles[PrevQMLWidth] = "prevQMLWidth";
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
    if (role == NextQMLWidth)
    {
        if (row + 1 > size() - 1)
        {
            return 0;
        }
        if (!sameTime(*getObject(row), *getObject(row + 1)))
        {
            return 0;
        }
        return getObject(row + 1)->qmlWidth();
    }
    if (role == NextSameTime)
    {
        if (row + 1 > size() - 1)
        {
            return false;
        }
        return sameTime(*getObject(row), *getObject(row + 1));
    }
    if (role == PrevQMLWidth)
    {
        if (row < 1)
        {
            return 0;
        }
        if (!sameTime(*getObject(row), *getObject(row - 1)))
        {
            return 0;
        }
        return getObject(row - 1)->qmlWidth();
    }
    if (role == PrevSameTime)
    {
        if (row < 1)
        {
            return false;
        }
        return sameTime(*getObject(row), *getObject(row - 1));
    }
    return QVariant();
}

bool MessagesModel::sameTime(const MessageObject &a,
                             const MessageObject &b) const
{
    if (a.senderId() != b.senderId())
    {
        return false;
    }
    return std::abs(a.sent().toSecsSinceEpoch() / 60 - b.sent().toSecsSinceEpoch() / 60) <= 1;

}

MessagesModel::MessagesModel(QQmlApplicationEngine &engine):
    TemplatedDataModel<MessageObject>(engine, "MessagesModel", "message", TemplatedDataModel<MessageObject>::reverse)
{

}

MessageObject *MessagesModel::at(int index) const
{
    return getObject(index);
}
