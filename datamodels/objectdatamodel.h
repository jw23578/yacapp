#ifndef OBJECTDATAMODEL_H
#define OBJECTDATAMODEL_H

#include "datamodelinterface.h"

class ObjectDataModel : public DataModelInterface<DataObjectInterface>
{
    std::vector<DataObjectInterface*> objects;

protected:
    size_t size() const override;
    DataObjectInterface *getObject(size_t index) const  override;
    void internalAppend(DataObjectInterface *object)  override;
    DataObjectInterface *previousObject(int index) const override;

public:
    ObjectDataModel(QQmlApplicationEngine &engine,
                    const QString &modelName,
                    const QString &objectName,
                    DirectionType const direction);
};

#endif // OBJECTDATAMODEL_H
