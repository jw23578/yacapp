#include "datamodelqobjectinterface.h"

DataModelQObjectInterface::DataModelQObjectInterface()
    : QAbstractItemModel()
{

}

void DataModelQObjectInterface::deleteById(const QString &id)
{
    internalDeleteById(id);
    setCount(size());
}
