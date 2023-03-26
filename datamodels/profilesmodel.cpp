#include "profilesmodel.h"

void ProfilesModel::internalAppend(ProfileObject *object)
{
    TemplatedDataModel<ProfileObject>::internalAppend(object);
    id2object[object->id()] = object;
}

void ProfilesModel::internalClear()
{
    id2object.clear();
    TemplatedDataModel<ProfileObject>::internalClear();
}

ProfilesModel::ProfilesModel(QQmlApplicationEngine &engine,
                             const QString &modelName):
    TemplatedDataModel<ProfileObject>(engine,
                                      modelName,
                                      "profile",
                                      TemplatedDataModel<ProfileObject>::forward)
{

}

void ProfilesModel::removeById(const QString &id)
{
    auto it(id2object.find(id));
    if (it == id2object.end())
    {
        return;
    }
    remove(it->second);
    id2object.erase(it);
}

ProfileObject *ProfilesModel::getCopyById(const QString &id) const
{
    auto it(id2object.find(id));
    if (it == id2object.end())
    {
        return 0;
    }
    return new ProfileObject(*it->second);
}

bool ProfilesModel::incUnreadMessages(const QString &id)
{
    auto it(id2object.find(id));
    if (it == id2object.end())
    {
        return false;
    }
    it->second->incUnreadMessages();
    bubbleSort(false,
               true,
               [](const ProfileObject &a, const ProfileObject &b){return a.lastAddedMessage() < b.lastAddedMessage();});
    return true;
}

bool ProfilesModel::contains(const QString &id) const
{
    return id2object.find(id) != id2object.end();
}

ProfileObject &ProfilesModel::getById(const QString &id)
{
    auto it(id2object.find(id));
    if (it != id2object.end())
    {
        return *it->second;
    }
    return emptyProfile;
}
