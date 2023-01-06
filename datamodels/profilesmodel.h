#ifndef PROFILESMODEL_H
#define PROFILESMODEL_H

#include "templateddatamodel.h"
#include "dataobjects/profileobject.h"

class ProfilesModel : public TemplatedDataModel<ProfileObject>
{
protected:
    std::map<QString, ProfileObject*> id2object;
    bool canAppend(ProfileObject *object) const override;
    void internalAppend(ProfileObject *object) override;
    void internalClear() override;
public:
    ProfilesModel(QQmlApplicationEngine &engine,
                  const QString &modelName);

    ProfileObject *getCopyById(const QString &id) const;
    bool incUnreadMessages(const QString &id);
};

#endif // PROFILESMODEL_H
