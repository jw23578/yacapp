#include "datamodelqobjectinterface.h"

DataModelQObjectInterface::DataModelQObjectInterface(QObject *parent)
    : QAbstractItemModel{parent}
{

}

void DataModelQObjectInterface::deleteById(const QString &id)
{
    internalDeleteById(id);
    setCount(size());
}
