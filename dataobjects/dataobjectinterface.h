#ifndef DATAOBJECTINTERFACE_H
#define DATAOBJECTINTERFACE_H

#include <QObject>
#include "yacappmacros.h"

class DataObjectInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataObjectInterface(QObject *parent = nullptr);

signals:

};

#endif // DATAOBJECTINTERFACE_H
