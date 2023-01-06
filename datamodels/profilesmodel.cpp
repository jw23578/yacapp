#include "profilesmodel.h"

bool ProfilesModel::canAppend(ProfileObject *object) const
{
    auto it(id2object.find(object->id()));
    return it == id2object.end();
}

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
    it->second->setUnreadMessages(it->second->unreadMessages() + 1);
    return false;
}
