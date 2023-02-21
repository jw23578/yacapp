#ifndef PROFILESMODEL_H
#define PROFILESMODEL_H

#include "templateddatamodel.h"
#include "dataobjects/profileobject.h"

class ProfilesModel : public TemplatedDataModel<ProfileObject>
{
    ProfileObject emptyProfile;
protected:
    std::map<QString, ProfileObject*> id2object;
    bool canAppend(ProfileObject *object) const override;
    void internalAppend(ProfileObject *object) override;
    void internalClear() override;
public:
    ProfilesModel(QQmlApplicationEngine &engine,
                  const QString &modelName);

    void removeById(const QString &id);
    ProfileObject *getCopyById(const QString &id) const;
    bool incUnreadMessages(const QString &id);

    bool contains(const QString &id) const;
    ProfileObject &getById(const QString &id);
};

#endif // PROFILESMODEL_H
