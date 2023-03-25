#ifndef DATAMODELQOBJECTINTERFACE_H
#define DATAMODELQOBJECTINTERFACE_H

#include <QAbstractItemModel>
#include "yacappmacros.h"

class DataModelQObjectInterface : public QAbstractItemModel
{
    Q_OBJECT
    YACAPPPROPERTY(int, count, Count, 0);
public:
    explicit DataModelQObjectInterface(QObject *parent = nullptr);
};

#endif // DATAMODELQOBJECTINTERFACE_H
