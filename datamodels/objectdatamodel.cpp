#include "objectdatamodel.h"

ObjectDataModel::ObjectDataModel(QQmlApplicationEngine &engine,
                                 const QString &modelName,
                                 const QString &objectName,
                                 DirectionType const direction):
    DataModelInterface(engine,
                       modelName,
                       objectName,
                       direction)
{

}

size_t ObjectDataModel::size() const
{
    return objects.size();
}

DataObjectInterface *ObjectDataModel::internalGetObject(size_t index) const
{
    return objects[static_cast<size_t>(index)];
}

void ObjectDataModel::internalAppend(DataObjectInterface *object)
{
    objects.push_back(object);
}

DataObjectInterface *ObjectDataModel::previousObject(int index) const
{
    if (index == 0)
    {
        return 0;
    }
    return objects[index - 1];
}
