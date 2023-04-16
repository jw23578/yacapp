#ifndef DATAMODELQOBJECTINTERFACE_H
#define DATAMODELQOBJECTINTERFACE_H

#include <QAbstractItemModel>
#include "yacappmacros.h"

class DataModelQObjectInterface : public QAbstractItemModel
{
    Q_OBJECT
    YACAPPPROPERTY(int, count, Count, 0);
protected:
    virtual void internalDeleteById(const QString &id) = 0;
public:
    explicit DataModelQObjectInterface(QObject *parent = nullptr);
    virtual ~DataModelQObjectInterface() {}

    virtual size_t size() const = 0;
    Q_INVOKABLE void deleteById(const QString &id);
};

#endif // DATAMODELQOBJECTINTERFACE_H
