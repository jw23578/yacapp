#ifndef DATAOBJECTINTERFACE_H
#define DATAOBJECTINTERFACE_H

#include <QObject>
#include "yacappmacros.h"
#include "yacAppAndServer/tablefields.h"


class DataObjectInterface : public QObject
{
    Q_OBJECT
protected:
    const TableFields tableFields;
public:
    explicit DataObjectInterface(QObject *parent = nullptr);

signals:

};

#endif // DATAOBJECTINTERFACE_H
